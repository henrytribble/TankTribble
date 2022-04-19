// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TankPawn.h"
#include "PlayerCont.generated.h"

/**
 * 
 */
UCLASS()
class TANKTRIBBLE_API APlayerCont : public APlayerController
{
	GENERATED_BODY()
	
#define Move 0
#define Steer 1
#define Shoot 2

private:
	void BeginPlay() override;

public:

	APlayerCont();

	UPROPERTY(EditAnywhere, Category = "Player");
	ATankPawn* playerPawn;

	UPROPERTY(EditAnywhere, Category = "Player");
	TArray<FTransform> spawns;
	
};
