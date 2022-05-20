// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Hazard.h"

// Sets default values
ASS_Hazard::ASS_Hazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Spawn_X_Vel = -500.0f;
	Spawn_Y_Vel = 0.0f;

	SelfDestructTimer = 1.0f;

	bHit = false;

}

// Called when the game starts or when spawned
void ASS_Hazard::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = 0.0f;
	Initial_X_Loc = 1500.0f;
	Initial_Y_Loc = FMath::RandRange(-350.0f, 350.0f);
	RandSize = FMath::RandRange(3, 8);
	this->SetActorScale3D(FVector(RandSize, RandSize, RandSize));

	OnActorBeginOverlap.AddDynamic(this, &ASS_Hazard::OnBeginOverlap);
}

// Called every frame
void ASS_Hazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(this != nullptr);

	InitialRotation += DeltaTime;

	Initial_X_Loc += DeltaTime * Spawn_X_Vel;
	Initial_Y_Loc += DeltaTime * Spawn_Y_Vel;

	this->SetActorRotation(FRotator(InitialRotation * 100.0f, InitialRotation * 50.0f, 0.0f));
	this->SetActorLocation(FVector(Initial_X_Loc, Initial_Y_Loc, 0.0f));

	if (SelfDestructTimer <= 0.0f)
		this->Destroy();

	if (bHit)
	{
		bHit = false;
		bStartDestroyTimer = true;

		this->StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);

	}
	if (bStartDestroyTimer)
		SelfDestructTimer -= DeltaTime;
}

void ASS_Hazard::SetHazardVelocity(FVector NewVelocity)
{
	Spawn_X_Vel = NewVelocity.X;
	Spawn_Y_Vel = NewVelocity.Y;
}

//Funcion que nos ayuda a saber si el hazard (Asteroide) choco con los limites, con un proyectile o con el jugador

void ASS_Hazard::OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Bounds"))
		SelfDestructTimer = 0.0f;

	if (OtherActor->ActorHasTag("Projectile") || OtherActor->ActorHasTag("Player"))
	{
		bHit = true;
	}
}
