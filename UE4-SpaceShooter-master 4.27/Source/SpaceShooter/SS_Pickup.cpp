// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Pickup.h"
#include "SS_Player.h"

// Sets default values
ASS_Pickup::ASS_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	PickUpMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts or when spawned
void ASS_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ASS_Pickup::OnBeginOverlap);
}

// Called every frame
void ASS_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = this->GetActorLocation();
	NewLocation.X -= DeltaTime * 100.0f;
	NewLocation.Z = 0;
	this->SetActorLocation(NewLocation);
}

//Funcion que nos ayuda a saber si nuestra nave choco con el pickup, si es el caso las castea y la toma con la funcion collectablePickup

void ASS_Pickup::OnBeginOverlap(AActor* PickupActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ASS_Player* PlayerActor = Cast<ASS_Player>(OtherActor);
		PlayerActor->CollectablePickup();
		this->Destroy();
	}
	if (OtherActor->ActorHasTag("Bounds"))
		this->Destroy();
}