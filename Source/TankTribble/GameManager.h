// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCont.h"
#include "MazeGenerator.h"
#include "EngineUtils.h"
#include "GameManager.generated.h"

UCLASS()
class TANKTRIBBLE_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	




	//Spawns in players, injects pawns
	void GameSetup();

	void StartRound();

	UPROPERTY(EditAnywhere, Category = "Player");
	int playerCount = 2;

	UPROPERTY(EditAnywhere, Category = "Player");
	TSubclassOf<ATankPawn> playerPawn;

	UPROPERTY(EditAnywhere, Category = "Player");
	TArray<APlayerCont*> players;

	UPROPERTY(EditAnywhere, Category = "Level");
	TArray<APlayerStart*> spawns;

	UPROPERTY(EditAnywhere, Category = "Level");
	AMazeGenerator* mazeGenerator;

	//templates
	
	//finds all the actors of type T in world
	template<typename T>
	void FindAllActors(UWorld* World, TArray<T*>& Out)
	{
		for (TActorIterator<T> It(World); It; ++It)
		{
			Out.Add(*It);
		}
	}

	//Instantiates 'playerCount' number of player controllers and injects relevant variables into them
	template<typename T>
	void SetUpPlayerController(int i)
	{
		APlayerCont* player = Cast<APlayerCont>(UGameplayStatics::CreatePlayer(GetWorld()));

		for (size_t j = 0; j < spawns.Num(); j++) //pass spawns to player
			player->spawns.Add(spawns[j]->GetTransform()); //give spawn positions

		player->playerPawn = GetWorld()->SpawnActor<ATankPawn>(playerPawn, player->spawns[i]); //spawn tank

		//inject references
		FName tag = FName("Player",i + 1);
		player->playerPawn->Tags.Add(tag);
		player->playerPawn->index = i;
		player->playerPawn->totalPlayers = playerCount;
		players.Add(player); //add to player list
	}

	//Alike to 'SetUpPlayerControllers' but skips a few processes as the first player controller is spawned in automatically
	template<typename T>
	void SetUpFirstPlayerController()
	{
		APlayerCont* firstPlayerCont =Cast<APlayerCont>( GetWorld()->GetFirstPlayerController());

		for (int j = 0; j < spawns.Num(); j++) //pass spawns to player
			firstPlayerCont->spawns.Add(spawns[j]->GetTransform());

		firstPlayerCont->playerPawn = GetWorld()->SpawnActor<ATankPawn>(playerPawn, firstPlayerCont->spawns[0]);

		firstPlayerCont->playerPawn->Tags.Add(FName("Player_0"));
		firstPlayerCont->playerPawn->index = 0;
		firstPlayerCont->playerPawn->totalPlayers = playerCount;

		players.Add(firstPlayerCont);
	}

	template <typename T>
	void RespawnPlayersPosition()
	{
		for (int i = 0; i < players.Num(); i++)
		{
			//get position for player to spawn
			AActor* pieceToSpawnOn = mazeGenerator->mazeObjs[FMath::RandRange(0, mazeGenerator->mazeObjs.Num())];
			
			//check spawn is not deadend, otherwise they would spawn inside a wall
			while(pieceToSpawnOn->GetClass() == mazeGenerator->deadEndPiece.Get())
				pieceToSpawnOn = mazeGenerator->mazeObjs[FMath::RandRange(0, mazeGenerator->mazeObjs.Num())];

			//get the position of chosen piece and spawn the player there
			FTransform mazePiecePos = pieceToSpawnOn->GetActorTransform();
			FVector spawnPos = FVector(mazePiecePos.GetLocation().X, mazePiecePos.GetLocation().Y, players[i]->playerPawn->GetActorLocation().Z);
			players[i]->playerPawn->SetActorLocation(spawnPos);
		}
	}
};
