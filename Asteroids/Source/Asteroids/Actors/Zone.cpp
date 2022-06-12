// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Zone.h"

// Sets default values
AZone::AZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_zoneComponent = CreateDefaultSubobject<USceneComponent>(FName("ZoneComponent"));
	SetRootComponent(m_zoneComponent);
}

// Called when the game starts or when spawned
void AZone::BeginPlay()
{
	Super::BeginPlay();
	
	// Set min and max corners in local space
	m_minCorner = -m_size;
	m_maxCorner = m_size;
}

// Called every frame
void AZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugOutline();
}

void AZone::DrawDebugOutline()
{
	TObjectPtr<UWorld> world = GetWorld();
	if (world == nullptr)
		return;

	DrawDebugBox(world, GetActorLocation(), m_size, FColor::Black, true, 1.f, (uint8)0U, 5.f);
}