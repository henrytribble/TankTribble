// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Cell.h"
#include "MazeGenerator.generated.h"



UCLASS()
class TANKTRIBBLE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
	//maze represented as a 2D array E.G:
/*
111111011111111
110000000110101
100111110100001
110000000001101
111010111010001
111111111111011

apolagies for awful diagram, 1s are walls and 0s are spaces 
this is an implementation of an 'imperfect' recursive backtracker algorithm, this is quite a well documented technique so you can 
easily see where I bungle it
*/


public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

protected:

	//maze data helper class
	class MazeData {
	public:
		//need reference to width and height properties used in init function as well as get & set
		MazeData(AMazeGenerator& MazeGen) : MazeGen{ MazeGen } {}

		// MazeGen.size.X = Width, MazeGen.size.Y = Height

		void Init()
		{
			Data.Init(1, MazeGen.size.X * MazeGen.size.Y);
		}

		//returns position on maze according to (x,y)
		int8 Get(int32 X, int32 Y) const
		{
			return Data[X + Y * MazeGen.size.X];
		}

		//sets walls on maze according to (x,y)
		void Set(int32 X, int32 Y, int8 value)
		{
			Data[X + Y * MazeGen.size.X] = value;
		}

	private:
		AMazeGenerator& MazeGen;
		TArray<int8> Data;
	};
	
	MazeData Maze = MazeData(*this);

	class WorldDirections {
	public:
		//shuffles elements in data array, See "Fisher-Yates Algorithm" for more info
		void Shuffle()
		{
			for (int32 i = Data.Num() - 1; i != 0; i--)
			{
				int32 j = rand() % i;
				Data.Swap(i, j);
			}
		}

		int8& operator[](size_t i) { return Data[i]; }

		int32 Num() { return Data.Num(); }
	private:

		// 0 = North, 1 = East, 2 = South, 3 = West
		TArray<int8> Data = { 0,1,2,3 };
	};

	WorldDirections Directions;
	

	//recursive funtion that steps through maze, initially filled with 1s and replaces them with 0s
	void Step(int32 X, int32 Y);

	//loops through maze array, and places pieces
	void Draw();

	//places the pieces in the correct orientation
	void PlacePiece(int32 X, int32 Y, float yaw, TSubclassOf<AActor> piece);

	void CreateMaze();

	//checks for matching spot in the maze 
	bool IsPatternMatching(int32 X, int32 Y, TArray<int8> pattern) const;

	//templates
	template <typename T>
	void DestroyOldMaze()
	{
		for (int i = 0; i < mazeObjs.Num(); i++)
		{
			mazeObjs[i]->Destroy();
		}
		mazeObjs.Empty();
	}




#pragma region patterns
	//5s and 1s are walls, 0s are paths
	//5 is 'wildcard' does not need to 5, but is chosen for its visual difference to both 1 and 0

	// Straight
	TArray<int8> HorizontalStraightPattern = { 5, 1, 5,
											   0, 0, 0,
											   5, 1, 5 };

	/*
	* can be read like this:
	Whatever  Wall  Whatever
	Path      Path      Path
	Whatever  Wall  Whatever
	*/

	TArray<int8> VerticalStraightPattern = { 5, 0, 5,
											 1, 0, 1,
											 5, 0, 5 };

	// T Junctions	
	TArray<int8> TJunctionUpPattern = { 1, 0, 1,
										0, 0, 0,
										5, 1, 5 };

	TArray<int8> TJunctionDownPattern = { 5, 1, 5,
										  0, 0, 0,
										  1, 0, 1 };

	TArray<int8> TJunctionLeftPattern = { 1, 0, 5,
										  0, 0, 1,
										  1, 0, 5 };

	TArray<int8> TJunctionRightPattern = { 5, 0, 1,
										   1, 0, 0,
										   5, 0, 1 };

	// Crossroad
	TArray<int8> CrossroadPattern = { 1, 0, 1,
									  0, 0, 0,
									  1, 0, 1 };

	// Turns
	TArray<int8> TurnLeftUpPattern = { 1, 0, 5,
									   0, 0, 1,
									   5, 1, 5 };

	TArray<int8> TurnLeftDownPattern = { 5, 1, 5,
										 0, 0, 1,
										 1, 0, 5 };

	TArray<int8> TurnRightUpPattern = { 5, 0, 1,
										1, 0, 0,
										5, 1, 5 };

	TArray<int8> TurnRightDownPattern = { 5, 1, 5,
										  1, 0, 0,
										  5, 0, 1 };

	// Dead ends
	TArray<int8> DeadEndUpPattern = { 5, 0, 5,
									  1, 0, 1,
									  5, 1, 5 };

	TArray<int8> DeadEndDownPattern = { 5, 1, 5,
										1, 0, 1,
										5, 0, 5 };

	TArray<int8> DeadEndLeftPattern = { 5, 1, 5,
										0, 0, 1,
										5, 1, 5 };

	TArray<int8> DeadEndRightPattern = { 5, 1, 5,
										 1, 0, 0,
										 5, 1, 5 };

#pragma endregion


public:	


	void NewMaze();

	TArray<AActor*> mazeObjs;

	UPROPERTY(EditAnywhere, Category = "Level");
	FVector2D size = FVector2D(100,100);

	UPROPERTY(EditAnywhere, Category = "Level");
	FVector2D start = FVector2D(5, 5);

	UPROPERTY(EditAnywhere, Category = "Level");
	int32 scale = 300;

#pragma region pieces
	UPROPERTY(EditAnywhere, Category = "Level");
	TSubclassOf<AActor> straightPiece;

	UPROPERTY(EditAnywhere, Category = "Level");
	TSubclassOf<AActor> TJunctionPiece;

	UPROPERTY(EditAnywhere, Category = "Level");
	TSubclassOf<AActor> crossRoadPiece;

	UPROPERTY(EditAnywhere, Category = "Level");
	TSubclassOf<AActor> turnPiece;

	UPROPERTY(EditAnywhere, Category = "Level");
	TSubclassOf<AActor> deadEndPiece;
#pragma endregion
};


