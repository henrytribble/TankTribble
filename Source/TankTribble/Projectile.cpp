// Fill out your copyright notice in the Description page of Project Settings.
#include "Projectile.h"
#include "TankPawn.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set up collision
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	boxCollider->SetSimulatePhysics(true);
	boxCollider->SetNotifyRigidBodyCollision(true);
	boxCollider->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	boxCollider->OnComponentHit.AddDynamic(this, &AProjectile::OnCollisionEnter);

	//set collider as root so that collisions work as intended
	RootComponent = boxCollider;
}

// Called every frame
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer += DeltaTime;
	if(timer >= fuse)
		Reset();

}

void AProjectile::Reset()
{
	parent->ammo++;
	Destroy();
}

void AProjectile::Bounce()
{
	SpawnExplosion<UParticleSystem>(explosion);
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion, GetActorLocation(), GetActorRotation());

	bounces--;
	if (bounces <= 0)
	{
		Reset();
	}
}



void AProjectile::OnCollisionEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		ATankPawn* player = Cast<ATankPawn>(OtherActor);
		if (player)
		{
			parent->ChangeScore(player == this->parent ? -1 : 1);
			
			player->ChangeHealth(damage);
			GetWorld()->SpawnActor<AActor>(coupDeGrace,OtherActor->GetActorTransform());
			
			Reset();
		}
		else
			Bounce();


	}
}

