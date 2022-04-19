// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Projectile_Default.generated.h"

/**
 * 
 */
UCLASS()
class TANKTRIBBLE_API AProjectile_Default : public AProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile_Default();

	void BeginPlay();

	void Tick(float DeltaTime);
};
