// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "ShipBase.generated.h"

class UPawnMovementComponent;

UCLASS()
class ASTEROIDS_API AShipBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShipBase();

	// Input Event Handlers
	virtual void OnMoveForward(float axisValue);
	virtual void OnMoveBackward(float axisValue);
	virtual void OnMoveRightward(float axisValue);
	virtual void OnMoveLeftward(float axisValue);
	virtual void OnMoveUpward(float axisValue);
	virtual void OnMoveDownward(float axisValue);
	virtual void OnRollClockwise(float axisValue);
	virtual void OnRollCounterClockwise(float axisValue);
	virtual void OnMouseMove(FVector axisValue);
	virtual void OnMouseWheel(float axisValue);
	virtual void OnMouseMoveX(float axisValue);
	virtual void OnMouseMoveY(float axisValue);

	UFUNCTION()
		void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
		void OnEndOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DrawDebugAxes(float DeltaTime);

	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> m_visibleComponent;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPawnMovementComponent> m_movementComponent;
	UPROPERTY(VisibleAnywhere) // To be changed to a camera on a spring arm
		TObjectPtr<UCameraComponent> m_cameraComponent;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USphereComponent> m_collisionComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
