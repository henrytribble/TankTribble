// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS()
class TANKTRIBBLE_API ACell : public AActor
{
	GENERATED_BODY()
	
#define UP = 0x1000;
#define DOWN = 0x0100;
#define LEFT = 0x010;
#define RIGHT = 0x0001;
private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:	
	// Sets default values for this actor's properties
	ACell();
	UPROPERTY(EditAnywhere, Category = "Level");
	bool bVisited = false;

	UPROPERTY(EditAnywhere, Category = "Level");
	unsigned int walls = 0x0000;
};
