// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

// Sets default values
AGameManager::AGameManager()
{

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
    FindAllActors(GetWorld(), spawns);
	GameSetup();
	StartRound();
}

void AGameManager::GameSetup()
{

	//first player controller auto spawns so deal with seperately to renegade controllers
	SetUpFirstPlayerController<void>();

	//deal with renegade controllers
	for (size_t i = 1; i < playerCount; i++)
	{
		SetUpPlayerController<void>(i);
	}
}

void AGameManager::StartRound()
{
	mazeGenerator->NewMaze();
	//reset players
	RespawnPlayersPosition<void>();
}




