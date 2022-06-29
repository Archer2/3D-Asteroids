// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zone.generated.h"

UCLASS()
class ASTEROIDS_API AZone : public AActor
{
	GENERATED_BODY()
	
protected:
	void DrawDebugOutline(float DeltaTime);

	UPROPERTY(EditAnywhere)
		FVector m_size;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> m_zoneComponent;

	FVector m_minCorner;
	FVector m_maxCorner;

public:	
	// Sets default values for this actor's properties
	AZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
