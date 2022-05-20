// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Player.h"
#include "SS_GameMode.h"
#include "Kismet/GameplayStatics.h"

ASS_Player::ASS_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Nos permite tomar control de nuestra nave
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ShipMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MaxVelocity = 250.0f;
	Current_X_Vel = Current_Y_Vel = 0.0f;
	bIsFiring = false;
	WeaponFireRate = 280.0f; 
	TimeSinceLastShot = 0.0f;
	PlayerScore = 0.0f;

	const FVector MoveDirection = FVector(FireForwardValue, FireRightValue, 0.f).GetClampedToMaxSize(1.0f);

	const FRotator FireRotation = MoveDirection.Rotation();
	const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		// spawn the projectile

		//ColaProyectiles.Push(World->SpawnActor<AProjectile>(SpawnLocation, FireRotation));
		//ColaProyectiles.Push(World->SpawnActor<AProjectile>(SpawnLocation, FireRotation));
		//ColaProyectiles.Push(World->SpawnActor<AProjectile>(SpawnLocation, FireRotation));
		//ColaProyectiles.Push(World->SpawnActor<AProjectile>(SpawnLocation, FireRotation));
		//ColaProyectiles.Push(World->SpawnActor<AProjectile>(SpawnLocation, FireRotation));

	}
}

void ASS_Player::MoveRight(float AxisValue)
{
	Current_X_Vel = MaxVelocity * AxisValue;
}

void ASS_Player::MoveUp(float AxisValue)
{
	Current_Y_Vel = MaxVelocity * AxisValue;
}

void ASS_Player::UpdateMovement(float DeltaTime)
{
	if (Current_X_Vel != 0.0f || Current_Y_Vel != 0.0f)
	{
		New_Location = FVector(Current_Location.X + (Current_X_Vel * DeltaTime),
			Current_Location.Y + (Current_Y_Vel * DeltaTime), 0);

		this->SetActorLocation(New_Location);

		Current_Location = New_Location;

		//cuadrado del cual no podra salir nuestra nave
		if (this->GetActorLocation().X > Field_Width)
			Current_Location = FVector(Field_Width - 1, Current_Location.Y, Current_Location.Z);
		if (this->GetActorLocation().X < -Field_Width)
			Current_Location = FVector(-Field_Width + 1, Current_Location.Y, Current_Location.Z);
		if (this->GetActorLocation().Y > Field_Heigth)
			Current_Location = FVector(Current_Location.X, Field_Heigth - 1, Current_Location.Z);
		if (this->GetActorLocation().Y < -Field_Heigth)
			Current_Location = FVector(Current_Location.X, -Field_Heigth + 1, Current_Location.Z);
	}
	// Rotacion de la nave cuando se esta moviendo
	if (Current_Y_Vel > 0.1f)
		this->SetActorRotation(FRotator(Current_Rotation.Pitch + 45.0f,
			Current_Rotation.Yaw,
			Current_Rotation.Roll));
	else if (Current_Y_Vel < -0.1f)
		this->SetActorRotation(FRotator(Current_Rotation.Pitch - 45.0f,
			Current_Rotation.Yaw,
			Current_Rotation.Roll));
	else
		this->SetActorRotation(Current_Rotation);
}

//Funciones relacionadas al disparo de nuestra nave

void ASS_Player::UpdateFiring(float DeltaTime)
{
	if (bIsFiring && TimeSinceLastShot > ( 60 / WeaponFireRate ))
	{
		FireWeapon();
		TimeSinceLastShot = 0.0f;
	}
	TimeSinceLastShot += DeltaTime;
}

void ASS_Player::StartFiring()
{
	bIsFiring = true;
}

void ASS_Player::StopFiring()
{
	bIsFiring = false;
}

void ASS_Player::FireWeapon()
{
	FActorSpawnParameters Params = {};
	Params.Owner = this;

	AActor* SpawnedProjectile = GetWorld()->SpawnActor(WeaponProjectile_BP, &Current_Location, &Current_Rotation, Params);

}

//Funcion que aumenta la armadura o vida de nuestra nave, y si no necesitamos vida o armadura nos aumenta el score
void ASS_Player::CollectablePickup()
{
	if (Current_Armor <= 100.0f && Current_Health == 100.0f)
	{
		Current_Armor += 10.0f;
		if (Current_Armor > 100.0f) Current_Armor = 100.0f;
	}
	else if (Current_Health <= 100.0f)
	{
		Current_Health += 10.0f;
		if (Current_Health > 100.0f) Current_Health = 100.0f;
	}
	else
		PlayerScore += 15.0f;
}

//Funcion que nos ayuda a reducir nuestra vida o armadura al chocar con un asteroide, un enemigo o un projectile

void ASS_Player::OnBeginOverlap(AActor* PlayerActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Asteroid") || OtherActor->ActorHasTag("EnemyShip"))
	{
		if (Current_Armor > 0.0f)
		{
			Current_Armor -= 10.0f;
			if (Current_Armor < 0.0f) Current_Armor = 0.0f;
		}
		else if (Current_Health > 0.0f)
		{
			Current_Health -= 10.0f;
			if (Current_Health < 0.0f)
			{
				Current_Health = 0.0f;
				bHit = true;
			}
		}
	}
	if (OtherActor->ActorHasTag("EnemyProjectile") )
	{
		if (Current_Armor > 0.0f)
		{
			Current_Armor -= 25.0f;
			if (Current_Armor < 0.0f) Current_Armor = 0.0f;
		}
		else if (Current_Health > 0.0f)
		{
			Current_Health -= 25.0f;
			if (Current_Health < 0.0f)
			{
				Current_Health = 0.0f;
				bHit = true;
			}
		}
	}
}

// Called when the game starts or when spawned
void ASS_Player::BeginPlay()
{
	Super::BeginPlay();

	bHit = bDead = false;
	Max_Health = Current_Health = 100.0f;
	Max_Armor = Current_Armor = 100.0f;
	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();
	OnActorBeginOverlap.AddDynamic(this, &ASS_Player::OnBeginOverlap);
}
// Called every frame
void ASS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateMovement(DeltaTime);
	UpdateFiring(DeltaTime);

	if (bHit)
	{
		bDead = true;
		this->ShipMesh->SetVisibility(false);

		this->SetActorTickEnabled(false);
		ASS_GameMode* GameModeRef = Cast<ASS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameModeRef->bPlayerDead = true;

	}


}

// Called to bind functionality to input
void ASS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASS_Player::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ASS_Player::MoveUp);

	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed, this, &ASS_Player::StartFiring);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Released, this, &ASS_Player::StopFiring);

}

