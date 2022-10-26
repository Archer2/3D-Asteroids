// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Zone.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

DEFINE_LOG_CATEGORY(LogZones);

// Sets default values
AZone::AZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_zoneComponent = CreateDefaultSubobject<UBoxComponent>(FName("Zone"));
	m_zoneComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(m_zoneComponent);
	
	//OnActorEndOverlap.AddDynamic(this, &AZone::OnEndOverlap);
}

FVector AZone::GetMaxPointWorld()
{
	return m_maxCorner;
}

FVector AZone::GetMinPointWorld()
{
	return m_minCorner;
}

void AZone::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OverlappedActor != this)
		return; // If something happened to hit this then it is very bad
}

// Called when the game starts or when spawned
void AZone::BeginPlay()
{
	Super::BeginPlay();
	
	// Set min and max corners in local space
	FVector size = m_zoneComponent->GetScaledBoxExtent();
	FVector position = GetActorLocation();
	m_minCorner = position - size;
	m_maxCorner = position + size;

	DrawDebugOutline(-1.f);
}

// Called every frame
void AZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugOutline(DeltaTime);
}

void AZone::DrawDebugOutline(float DeltaTime)
{
	TObjectPtr<UWorld> world = GetWorld();
	if (world == nullptr)
		return;

	DrawDebugBox(world, GetActorLocation(), (m_maxCorner - m_minCorner) / 2.f, FColor::Black, true, (DeltaTime > 0.f) ? DeltaTime : -1, (uint8)0U, 5.f);
}
