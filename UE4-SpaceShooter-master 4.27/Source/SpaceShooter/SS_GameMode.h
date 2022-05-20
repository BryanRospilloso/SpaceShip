// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SS_Enemy.h"
#include "SS_Hazard.h"
#include "Blueprint/UserWidget.h"
#include "SS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ASS_GameMode : public AGameMode
{
	GENERATED_BODY()

	//Agarramos nuestros blueprints para poder utilizarlos y crear un spawn

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASS_Enemy> Enemy;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASS_Hazard> HazardTemplate;

	ASS_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<ASS_Hazard> GetHazardTemplate();

public:
	int32 SpawnTimer;
	bool bPlayerDead;
	bool bStart;

protected:

	FActorSpawnParameters AsteroidSpawnParams;
	FVector AsteroidSpawnLocation;
	FRotator AsteroidSpawnRotation;

	UWorld* ThisWorld;

	//Declaramos nuestros widgets

	UPROPERTY(EditAnywhere, Category = "HUD Elements")
		TSubclassOf<UUserWidget> Score_Widget_Class;
	UUserWidget* Score_Widget;

	UPROPERTY(EditAnywhere, Category = "HUD Elements")
		TSubclassOf<UUserWidget> Health_And_Armor_Class;
	UUserWidget* Health_And_Armor_Widget;

	APlayerController* PC_Ref;
};
