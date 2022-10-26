// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Zone.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogZones, Log, All);

UCLASS()
class ASTEROIDS_API AZone : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AZone();

	FVector GetMaxPointWorld();
	FVector GetMinPointWorld();

	UFUNCTION()
		void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor); // Currently unused

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DrawDebugOutline(float DeltaTime);

	UPROPERTY(EditAnywhere)
		TObjectPtr<UBoxComponent> m_zoneComponent;

	FVector m_minCorner; // In World Space
	FVector m_maxCorner; // In World Space

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
