// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "SS_Hazard.generated.h"

UCLASS()
class SPACESHOOTER_API ASS_Hazard : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASS_Hazard();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UFUNCTION()
		void OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor);

	float InitialRotation;
	float Initial_X_Loc;
	float Initial_Y_Loc;
	float RandSize;
	float SelfDestructTimer;

	UPROPERTY(EditAnywhere)
		float Spawn_X_Vel;

	UPROPERTY(EditAnywhere)
		float Spawn_Y_Vel;

	bool bHit;
	bool bStartDestroyTimer;

	void SetHazardVelocity(FVector NewVelocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
