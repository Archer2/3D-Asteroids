// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ShipBase.h"
#include "Actors/Zone.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Runtime/Engine/Public/EngineGlobals.h"

DEFINE_LOG_CATEGORY(LogShip);

// Sets default values
AShipBase::AShipBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Type::Player0;

	// Setup Collision component (basic sphere as default) as the root component
	m_collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	m_collisionComponent->InitSphereRadius(25.f);
	// CollisionComponent values taken from reference default Third Person C++ Template
	m_collisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	m_collisionComponent->SetCanEverAffectNavigation(false);
	m_collisionComponent->SetShouldUpdatePhysicsVolume(true);
	m_collisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_collisionComponent->bDynamicObstacle = true;

	RootComponent = m_collisionComponent; // Assign to root

	// Setup Camera component
	m_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	m_cameraComponent->SetupAttachment(RootComponent);
	FVector defaultCameraLocation(-500.f, 0.f, 150.f);
	FQuat defaultCameraRotation(FVector3d(0.0, 1.0, 0.0), FMath::DegreesToRadians(5.f));
	m_cameraComponent->SetRelativeLocationAndRotation(defaultCameraLocation, defaultCameraRotation);

	// Setup Movement component
	m_movementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Movement Component"));
	m_movementComponent->UpdatedComponent = RootComponent;

	// Setup "Visible" component - the ship itself. Eventually this should also handle collision and be the Root
	m_visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Base"));
	//m_visibleComponent->bUseDefaultCollision = true; // Not working. Mesh has no default collision? Try with a different component
	m_visibleComponent->SetupAttachment(m_collisionComponent);

	// Set Event triggers
	//m_collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShipBase::OnBeginOverlapComponent);
	m_collisionComponent->OnComponentEndOverlap.AddDynamic(this, &AShipBase::OnEndOverlapComponent);
}

// Called when the game starts or when spawned
void AShipBase::BeginPlay()
{
	Super::BeginPlay();
}

void AShipBase::DrawDebugAxes(float DeltaTime)
{
	TObjectPtr<UWorld> world = GetWorld();
	if (world == nullptr)
		return;

	DrawDebugLine(world, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 100.f), FColor::Red, false, .05f, (uint8)0U, 2.f);
	DrawDebugLine(world, GetActorLocation(), GetActorLocation() + (GetActorRightVector() * 100.f), FColor::Green, false, .05f, (uint8)0U, 2.f);
	DrawDebugLine(world, GetActorLocation(), GetActorLocation() + (GetActorUpVector() * 100.f), FColor::Blue, false, .05f, (uint8)0U, 2.f);
}

void AShipBase::OnBeginOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogShip, Log, TEXT("Ship OnBeginOverlapComponent Called"));
}

void AShipBase::OnEndOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AZone* overlappedZone;
	if ((overlappedZone = Cast<AZone>(OtherActor)) != nullptr) {
		UE_LOG(LogShip, Log, TEXT("Ship OnEndOverlapComponent With Zone"));

		FVector zoneMaxPoint = overlappedZone->GetMaxPointWorld();
		FVector zoneMinPoint = overlappedZone->GetMinPointWorld();
		FVector shipPosition = GetActorLocation();
		FVector desiredShipPosition = shipPosition;

		// Check all dimensions for change and set the desired to the opposite coordinate
		if (shipPosition.X >= zoneMaxPoint.X) desiredShipPosition.X = zoneMinPoint.X;
		else if (shipPosition.X <= zoneMinPoint.X) desiredShipPosition.X = zoneMaxPoint.X;
		if (shipPosition.Y >= zoneMaxPoint.Y) desiredShipPosition.Y = zoneMinPoint.Y;
		else if (shipPosition.Y <= zoneMinPoint.Y) desiredShipPosition.Y = zoneMaxPoint.Y;
		if (shipPosition.Z >= zoneMaxPoint.Z) desiredShipPosition.Z = zoneMinPoint.Z;
		else if (shipPosition.Z <= zoneMinPoint.Z) desiredShipPosition.Z = zoneMaxPoint.Z;

		/* Disable the Movement Component Tick so as not to interfere with it's velocity updates in the middle of a Tick. Trigger a teleport and re-enable
		*  on the next Actor Tick
		*
		*  This must be done because this event could fire while the MovementComponent Tick is operating. Usually this happens after it has cached the
		*  "Old Location" of the component and before it caches the "New Location". Since it uses the difference between these two values to calculate a new
		*  Velocity value this causes a massive jump in Velocity (a new magnitude on the order of 100-150k). The teleport must be delayed to a time where 
		*  the MovementComponent is not Ticking in order to maintain movement values (and in fact attempting to reset them after teleporting here does not work).
		*/
		m_movementComponent->SetComponentTickEnabled(false);
		m_desiredTeleportLocation = desiredShipPosition;
		m_bTeleportNextTick = true;
	}
}

TObjectPtr<UPawnMovementComponent> AShipBase::GetMovementComponent()
{
	return m_movementComponent;
}

// Called every frame
void AShipBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Perform a cached teleport operation without interfering with Movement
	if (m_bTeleportNextTick) {
		TeleportTo(m_desiredTeleportLocation, GetActorRotation(), false, true);
		m_movementComponent->SetComponentTickEnabled(true);
		m_bTeleportNextTick = false;
	}

	// DrawDebugAxes(DeltaTime);
}

// Called to bind functionality to input
void AShipBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AShipBase::OnMoveForward);
	InputComponent->BindAxis("MoveBackward", this, &AShipBase::OnMoveBackward);
	InputComponent->BindAxis("MoveRightward", this, &AShipBase::OnMoveRightward);
	InputComponent->BindAxis("MoveLeftward", this, &AShipBase::OnMoveLeftward);
	InputComponent->BindAxis("MoveUpward", this, &AShipBase::OnMoveUpward);
	InputComponent->BindAxis("MoveDownward", this, &AShipBase::OnMoveDownward);
	InputComponent->BindAxis("RollClockwise", this, &AShipBase::OnRollClockwise);
	InputComponent->BindAxis("RollCounterClockwise", this, &AShipBase::OnRollCounterClockwise);

	InputComponent->BindAxis("MouseMoveX", this, &AShipBase::OnMouseMoveX); // Appended because OnMouseMove axisValue is always the ZeroVector
	InputComponent->BindAxis("MouseMoveY", this, &AShipBase::OnMouseMoveY);	// Appended because OnMouseMove axisValue is always the ZeroVector

	InputComponent->BindAxis("MouseWheel", this, &AShipBase::OnMouseWheel);
	//InputComponent->BindVectorAxis("MouseMove", this, &AShipBase::OnMouseMove); // Always ZeroVector ??
}

void AShipBase::OnMoveForward(float axisValue)
{	
	if (axisValue != 0.f) {
		AddMovementInput(GetActorForwardVector(), axisValue);
	}
}

void AShipBase::OnMoveBackward(float axisValue)
{
	if (axisValue != 0.f) {
		AddMovementInput(GetActorForwardVector(), -axisValue);
	}
}

void AShipBase::OnMoveRightward(float axisValue)
{
	if (axisValue != 0.f) {
		AddMovementInput(GetActorRightVector(), axisValue);
	}
}

void AShipBase::OnMoveLeftward(float axisValue)
{
	if (axisValue != 0.f) {
		AddMovementInput(GetActorRightVector(), -axisValue);
	}
}

void AShipBase::OnMoveUpward(float axisValue)
{
	if (axisValue != 0.f) {
		AddMovementInput(GetActorUpVector(), axisValue);
	}
}

void AShipBase::OnMoveDownward(float axisValue)
{
	if (axisValue != 0.f) {
		AddMovementInput(GetActorUpVector(), -axisValue);
	}
}

void AShipBase::OnRollClockwise(float axisValue)
{
	FQuat rotation(GetActorQuat() * FQuat(FVector3d(1.0, 0.0, 0.0), FMath::DegreesToRadians(-axisValue)));
	SetActorRotation(rotation);
}

void AShipBase::OnRollCounterClockwise(float axisValue)
{
	FQuat rotation(GetActorQuat() * FQuat(FVector3d(1.0, 0.0, 0.0), FMath::DegreesToRadians(axisValue)));
	SetActorRotation(rotation);
}

void AShipBase::OnMouseMove(FVector axisValue)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Black, FString::Printf(TEXT("OnMouseMove axisValue: %s"), *axisValue.ToString()));
}

void AShipBase::OnMouseWheel(float axisValue)
{
	// Not implemented. Possibly change max Speed? (Once acceleration, velocity, and speed are in)
}

// Temporary function that is a single-axis callback instead of Mouse XY 2D Axis
void AShipBase::OnMouseMoveX(float axisValue)
{
	FQuat rotation(GetActorQuat() * FQuat(FVector3d(0.0, 0.0, 1.0), FMath::DegreesToRadians(axisValue)));
	SetActorRotation(rotation);
}

// Temporary function that is a single-axis callback instead of Mouse XY 2D Axis
void AShipBase::OnMouseMoveY(float axisValue)
{
#define IS_INVERTED_Y true
	if (IS_INVERTED_Y) axisValue = -axisValue; // Placeholder for more formal InvertYAxis controls

	FQuat rotation(GetActorQuat() * FQuat(FVector3d(0.0, 1.0, 0.0), FMath::DegreesToRadians(axisValue)));
	SetActorRotation(rotation);
}
