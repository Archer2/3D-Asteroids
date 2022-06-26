// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ShipBase.h"

// Sets default values
AShipBase::AShipBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Type::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	m_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	m_cameraComponent->SetupAttachment(RootComponent);
	FVector defaultCameraLocation(-250.f, 0.f, 250.f);
	FQuat defaultCameraRotation(FVector3d(0.0, 1.0, 0.0), FMath::DegreesToRadians(45));

	m_cameraComponent->SetRelativeLocationAndRotation(defaultCameraLocation, defaultCameraRotation);
	m_visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Base"));
	m_visibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShipBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShipBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShipBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
