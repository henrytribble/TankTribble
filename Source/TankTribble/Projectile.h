// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.generated.h"


class ATankPawn;

UCLASS()
class TANKTRIBBLE_API AProjectile : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "Projectile");
	float fuse = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile");
	int bounces = 4;

	UPROPERTY(EditAnywhere, Category = "Projectile");
	int damage = 100;

	UPROPERTY(VisibleAnywhere, Category = "Projectile");
	UBoxComponent* boxCollider;

	UPROPERTY(EditAnywhere, Category = "Projectile");
	UParticleSystem* explosion;

	UPROPERTY(EditAnywhere, Category = "Projectile");
	UParticleSystem* sparks;

	UPROPERTY(EditAnywhere, Category = "Projectile");
	ATankPawn* parent;

	// Called every frame
	void Tick(float DeltaTime) override;

	virtual void Bounce();

	UFUNCTION()
	void OnCollisionEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Reset();

	template< class T >
	T* SpawnExplosion
	(
		UParticleSystem* explosion 
	)
	{
		return (T*)(UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion, GetActorLocation(), GetActorRotation()));
	}
};
