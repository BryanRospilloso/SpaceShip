// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "Clase.h"
#include "SS_Player.generated.h"

UCLASS()
class SPACESHOOTER_API ASS_Player : public APawn
{
	GENERATED_BODY()
		Cola<class AProjectile*> ColaProyectiles;

public:
	
	ASS_Player();

	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ShipMesh;
	
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	//Variables que nos permitiran establecer un cuadrado dentro del proyecto de la cual la nave no podra salir
	
	UPROPERTY(EditAnywhere)
		float Field_Width;
	UPROPERTY(EditAnywhere)
		float Field_Heigth;

	//Variables que nos permitiran manejar la vida maxima de nuestra nave, la vida actual, la maxima armadura y la armadura actual

	UPROPERTY(BlueprintReadOnly)
		float Max_Health;
	UPROPERTY(BlueprintReadOnly)
		float Current_Health;
	UPROPERTY(BlueprintReadOnly)
		float Max_Armor;
	UPROPERTY(BlueprintReadOnly)
		float Current_Armor;

	//Funciones para el movimiento de la nave

	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);
	void UpdateMovement(float DeltaTime);

	//Funciones para el disparo de la nave

	void UpdateFiring(float DeltaTime);
	void FireWeapon();
	void StartFiring();
	void StopFiring();

	//Funcion que nos permitira aumentar la vida y armadura de nuestra nave cuando recojamos un objeto

	void CollectablePickup();

	//Variables para controlar distintos parametros a la otra de hacer que nuestra nave se mueva y dispare

	UPROPERTY(EditAnywhere)
		float MaxVelocity;

	bool bIsFiring;
	bool bHit;
	bool bDead;

	float Current_X_Vel;
	float Current_Y_Vel;

	float WeaponFireRate;
	float TimeSinceLastShot;

	float FireForwardValue;
	float FireRightValue;

	//Variable que nos permite controlar el score

	UPROPERTY(BlueprintReadOnly)
		float PlayerScore;

	FVector Current_Location;
	FVector New_Location;
	FRotator Current_Rotation;

	UFUNCTION()
		void OnBeginOverlap(AActor * PlayerActor, AActor * OtherActor );

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> WeaponProjectile_BP;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FVector GunOffset;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
