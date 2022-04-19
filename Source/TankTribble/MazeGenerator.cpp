// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMazeGenerator::NewMaze()
{
    //if maze is present 
    DestroyOldMaze<void>();
    CreateMaze();
}

void AMazeGenerator::CreateMaze()
{
    Maze.Init();
    Step(start.X, start.Y);
    Draw();
}

void AMazeGenerator::Step(int32 X, int32 Y)
{
	//size.X = width, size.Y = height

	//first shuffle directions
	Directions.Shuffle();
	//E.G { 0, 3, 1, 2 }; Up (+X), Left (-Y), Down (-X), Right (+Y) so as to randomise the maze somewhat

	//loop over our directions to set the maze up
	for (size_t i = 0; i < Directions.Num(); i++)
	{
		switch (Directions[i])
		{
        case 0: // Up
            if (X + 2 >= size.X - 1 || Maze.Get(X + 2, Y) == 0)
                continue;

            Maze.Set(X + 2, Y, 0);
            Maze.Set(X + 1, Y, 0);
            Step(X + 2, Y);

        case 1: // Right
            if (Y + 2 >= size.Y - 1 || Maze.Get(X, Y + 2) == 0)
                continue;

            Maze.Set(X, Y + 2, 0);
            Maze.Set(X, Y + 1, 0);
            Step(X, Y + 2);

        case 2: // Down
            if (X - 2 <= 0 || Maze.Get(X - 2, Y) == 0)
                continue;

            Maze.Set(X - 2, Y, 0);
            Maze.Set(X - 1, Y, 0);
            Step(X - 2, Y);

        case 3: // Left
            if (Y - 2 <= 0 || Maze.Get(X, Y - 2) == 0)
                continue;

            Maze.Set(X, Y - 2, 0);
            Maze.Set(X, Y - 1, 0);
            Step(X, Y - 2);
		}
	}
}

void AMazeGenerator::Draw()
{
    for (int32 x = 1; x < size.X - 1; x++)
    {
        for (int32 y = 1; y < size.Y - 1; y++)
        {
            // Straight
            if (IsPatternMatching(x, y, HorizontalStraightPattern)) { PlacePiece(x, y, 90.f, straightPiece); }
            else if (IsPatternMatching(x, y, VerticalStraightPattern)) { PlacePiece(x, y, 0.f, straightPiece); }

            // Turns
            else if (IsPatternMatching(x, y, TurnLeftUpPattern)) { PlacePiece(x, y, 0.f, turnPiece); }
            else if (IsPatternMatching(x, y, TurnLeftDownPattern)) { PlacePiece(x, y, 90.f, turnPiece); }
            else if (IsPatternMatching(x, y, TurnRightUpPattern)) { PlacePiece(x, y, -90.f, turnPiece); }
            else if (IsPatternMatching(x, y, TurnRightDownPattern)) { PlacePiece(x, y, 180.f, turnPiece); }

            // T Junctions	
            else if (IsPatternMatching(x, y, TJunctionUpPattern)) { PlacePiece(x, y, -90.f, TJunctionPiece); }
            else if (IsPatternMatching(x, y, TJunctionDownPattern)) { PlacePiece(x, y, 90.f, TJunctionPiece); }
            else if (IsPatternMatching(x, y, TJunctionLeftPattern)) { PlacePiece(x, y, 0.f, TJunctionPiece); }
            else if (IsPatternMatching(x, y, TJunctionRightPattern)) { PlacePiece(x, y, 180.f, TJunctionPiece); }

            // Dead ends
            else if (IsPatternMatching(x, y, DeadEndUpPattern)) { PlacePiece(x, y, 90.f, deadEndPiece); }
            else if (IsPatternMatching(x, y, DeadEndDownPattern)) { PlacePiece(x, y, -90.f, deadEndPiece); }
            else if (IsPatternMatching(x, y, DeadEndLeftPattern)) { PlacePiece(x, y, 180.f, deadEndPiece); }
            else if (IsPatternMatching(x, y, DeadEndRightPattern)) { PlacePiece(x, y, 0.f, deadEndPiece); }

            // Crossroad
            else if (IsPatternMatching(x, y, CrossroadPattern)) { PlacePiece(x, y, 0.f, crossRoadPiece); }
        }
    }
}

bool AMazeGenerator::IsPatternMatching(int32 X, int32 Y, TArray<int8> pattern) const
{
    int count = 0;
    int i = 0;
    for (int y = 1; y > -2; y--)
    {
        for (int x = -1; x < 2; x++)
        {
            if (pattern[i] == Maze.Get(X + x, Y + y) || pattern[i] == 5)
            {
                count++;
            }
            i++;
        }
    }
    return count == 9;
}

void AMazeGenerator::PlacePiece(int32 X, int32 Y, float yaw, TSubclassOf<AActor> piece)
{
    FVector location(X * scale, Y * scale, 0);
    FRotator rotation(0, yaw, 0);
    FActorSpawnParameters spawnInfo;

    AActor *obj = GetWorld()->SpawnActor<AActor>(piece, location, rotation, spawnInfo);

    mazeObjs.Add(obj);
}


