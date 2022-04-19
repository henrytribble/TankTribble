// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	muzzlePos = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Muzzle")));
}


// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ATankPawn::Drive(DeltaTime);
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("g %s")));
}

#pragma region Abilities
void ATankPawn::Drive(float deltaTime)
{
	//steer
	AddControllerYawInput(driveInputs.X * rotSpeed * deltaTime);
	//move	
	AddMovementInput(GetActorForwardVector(), driveInputs.Y * moveSpeed * deltaTime);
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);
	if(index == 0)
		BindKeys(inputComponent);

}

void ATankPawn::BindKeys(UInputComponent* inputComponent)
{
	//bind player one
	inputComponent->BindAction(inputs[0][Shoot], IE_Pressed, this, &ATankPawn::Fire);
	inputComponent->BindAxis(inputs[0][Move], this, &ATankPawn::MoveInput);
	inputComponent->BindAxis(inputs[0][Steer], this, &ATankPawn::SteerInput);

	//bind all other players
	for (int i = 1; i < totalPlayers; i++)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player", i + 1), actors);
		inputComponent->BindAction(inputs[i][Shoot], IE_Pressed, Cast<ATankPawn>(actors[0]), &ATankPawn::Fire);
		inputComponent->BindAxis(inputs[i][Move], Cast<ATankPawn>(actors[0]), &ATankPawn::MoveInput);
		inputComponent->BindAxis(inputs[i][Steer], Cast<ATankPawn>(actors[0]), &ATankPawn::SteerInput);
	}
}

void ATankPawn::MoveInput(float value)
{
	driveInputs.Y = value;
}

void ATankPawn::SteerInput(float value)
{
	driveInputs.X = value;
}

void ATankPawn::Fire()
{
	if (projectile && ammo > 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ammo: %d"), ammo));
		FTransform _muzzle = muzzlePos->GetComponentTransform();
		AProjectile* projInstance = GetWorld()->SpawnActor<AProjectile>(projectile, _muzzle);
		projInstance->parent = this;
		ammo--;
	}
}

void ATankPawn::ChangeHealth(int changeAmount)
{
	health -= changeAmount;

	if (health <= 0)
		Explode();
}

void ATankPawn::ChangeScore(int changeAmount)
{
	score += changeAmount;
}

void ATankPawn::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Player is dead"));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Hit Player: %s"), *this->GetName()));
	
	//respawn
}

#pragma endregion

