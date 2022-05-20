// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_GameMode.h"

ASS_GameMode::ASS_GameMode()
{
	SpawnTimer = 0.0f;

	AsteroidSpawnLocation = FVector(1000.0f, 1000.0f, 1000.0f);
	AsteroidSpawnRotation = FRotator(0.0f, 0.0f,0.0f);

	ThisWorld = GetWorld();
}

void ASS_GameMode::BeginPlay()
{
	Super::BeginPlay();
	bPlayerDead = false;

	//Implementamos nuestros widgets

	if (Score_Widget_Class)
	{
		Score_Widget = CreateWidget<UUserWidget>(GetWorld(), Score_Widget_Class);
		Score_Widget->AddToViewport();
	}

	if (Health_And_Armor_Class)
	{
		Health_And_Armor_Widget = CreateWidget<UUserWidget>(GetWorld(), Health_And_Armor_Class);
		Health_And_Armor_Widget->AddToViewport();
	}

	PC_Ref = GetWorld()->GetFirstPlayerController();


}

void ASS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer = FMath::RandRange(0, 1000);

	//Le damos un spawn a nuestros enemigos y asteroides

	if (Enemy && SpawnTimer > 990)
	{
		FVector EnemySpawnLoc(0.0f, 0.0f, 0.0f);
		FRotator EnemySpawnRot(0.0f, 0.0f, 0.0f);

		AActor* NewEnemy = GetWorld()->SpawnActor(Enemy, &EnemySpawnLoc, &EnemySpawnRot, AsteroidSpawnParams);

		if (NewEnemy)
			NewEnemy->SetActorLocation(FVector(1000.0f, FMath::RandRange(-500,500), 0.0f));	
	}

	if (HazardTemplate && SpawnTimer > 995)
		GetWorld()->SpawnActor(HazardTemplate, &AsteroidSpawnLocation, &AsteroidSpawnRotation, AsteroidSpawnParams);	

}

TSubclassOf<ASS_Hazard> ASS_GameMode::GetHazardTemplate()
{
	return TSubclassOf<ASS_Hazard>();
}