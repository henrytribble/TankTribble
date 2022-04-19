// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKTRIBBLE_API ATankPawn : public APawn
{
	GENERATED_BODY()



#define Move 0
#define Steer 1
#define Shoot 2

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UPROPERTY(EditAnywhere, Category = "Player");
	int health = 100;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float moveSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float rotSpeed;

	UPROPERTY(EditAnywhere, Category = "Firepower");
	TSubclassOf<class AProjectile> projectile;


	UPROPERTY(EditAnywhere, Category = "Firepower");
	USceneComponent* muzzlePos;
	
	UPROPERTY(EditAnywhere, Category = "Firepower");
	int ammo = 1;

	UPROPERTY(EditAnywhere, Category = "Firepower");
	int index = 0;

	UPROPERTY(EditAnywhere, Category = "Firepower");
	int totalPlayers = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float zRot;

	FVector moveDir;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector2D driveInputs;

	virtual void SetupPlayerInputComponent(UInputComponent* inputComponent) override;

	FName inputs[3][3] = { {"POneMove","POneSteer","POneShoot"},{"PTwoMove","PTwoSteer","PTwoShoot"},{"PThreeMove","PThreeSteer","PThreeShoot"} };
#pragma region diagram
	// .-----------.-------------.--------------.---------------.
	// |           | Player One  | Player Two   | Player Three  |
	// :-----------+-------------+--------------+---------------:
	// | Movement  | POneMove    | PTwoMove     | PThreeMove    |
	// :-----------+-------------+--------------+---------------:
	// | Steering  | POneSteer   | PTwoSteer    | PThreeSteer   |
	// :-----------+-------------+--------------+---------------:
	// | Shooting  | POneShoot   | PTwoShoot    | PThreeShoot   |
	// '-----------'-------------'--------------'---------------' 
#pragma endregion

#pragma region Abilities

	void BindKeys(UInputComponent* inputComponent);

	void MoveInput(float value);

	void SteerInput(float value);

	void Drive(float deltaTime);

	UFUNCTION(BlueprintCallable, Category = "Firepower")
	void Fire();

	void ChangeHealth(int changeAmount);

	void Explode();

#pragma endregion
};
