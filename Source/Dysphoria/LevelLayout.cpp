// Copyright T Bilodeau 2020

#include "LevelLayout.h"

#include "LevelData.h"
#include "RoomData.h"

#include <random>
#include <time.h>
#include <cmath>

void ULevelLayout::Setup(const int32 RowRooms, const int32 ColRooms)
{
	uint32 RndSeed = time(NULL);
	srand(RndSeed);
	LevelData = NewObject<ULevelData>();
	LevelData->InitializeLevelDataMap(RowRooms, ColRooms, RndSeed);
}

void ULevelLayout::Build()
{
	if (!LevelData) { return; }

	//Connects all the rooms with doors
	ConnectAllRooms();
	
	//Let's add extra doors to make the map more navigatable and less labyrintian
	AddExtraDoors();

	//Places important rooms
	PlaceEntranceRoom();
	PlaceBossRoom();
}

ULevelData* ULevelLayout::GetLevelData()
{
	if (!LevelData)
	{
		LevelData = NewObject<ULevelData>();
		LevelData->InitializeLevelDataMap(5, 5, 1);
		UE_LOG(LogTemp, Warning, TEXT("While getting level data in ULevelLayout, the map was not initialized. Returning an empty map."));
	}

	return LevelData;
}

// Using a greedy (PRIM) algorithm, connect all the rooms
void ULevelLayout::ConnectAllRooms() 
{
	FRoomLocation StartLocation = GetRandomLocation();
	TArray<FRoomLocation> Frontier;

	for (auto Neighbor : LevelData->GetNeighborsWithoutDoors(StartLocation)) {
		Frontier.Add(Neighbor);
	}

	//Looping through the frontier while there is one
	while (Frontier.Num() != 0) {
		// grab a random frontier room
		FRoomLocation SelectedFrontierRoom = Frontier[rand() % Frontier.Num()]; 

		// random bridge to a room already in the map
		FRoomLocation InMapNeighbor = GetNeighborToAttachTo(SelectedFrontierRoom);

		// add the door to the rooms
		LevelData->AddDoorsBetween(InMapNeighbor, SelectedFrontierRoom);

		// remove it from the frontier
		Frontier.Remove(SelectedFrontierRoom);

		// add any neighbor rooms without doors to the frontier (not already assigned)
		Frontier.Append(LevelData->GetNeighborsWithoutDoors(SelectedFrontierRoom));
	}

}

//Chosing a first row cell to be the start room
void ULevelLayout::PlaceEntranceRoom() 
{
	FRoomLocation Entrance;
	Entrance.Row = 0;
	Entrance.Col = rand() % LevelData->GetCols();

	LevelData->SetEntrance(Entrance);
}

//Choosing a last row cell to be the end room, connect every room around it, and then connect it just once randomly to one of those rooms
void ULevelLayout::PlaceBossRoom()
{
	FRoomLocation BossRoom;
	BossRoom.Row = LevelData->GetRows() - 1;
	BossRoom.Col = rand() % LevelData->GetCols();

	auto RoomsAroundBoss = LevelData->GetRoomsAround(BossRoom);

	//Add connections between every room next to each other
	for (auto& RoomAroundBoss : RoomsAroundBoss) {
		for (auto& ASecondRoom : RoomsAroundBoss) {
			if (abs(RoomAroundBoss.Row - ASecondRoom.Row) == 1 || abs(RoomAroundBoss.Col - ASecondRoom.Col) == 1) {
				LevelData->AddDoorsBetween(RoomAroundBoss, ASecondRoom);
			}
		}
		//Preemptively disconnect the rooms from the boss room
		LevelData->RemoveDoorsBetween(BossRoom, RoomAroundBoss);
	}

	auto& RoomToConnect = RoomsAroundBoss[rand() % RoomsAroundBoss.Num()];

	LevelData->AddDoorsBetween(BossRoom, RoomToConnect);
	LevelData->SetBossRoom(BossRoom);
}

void ULevelLayout::AddExtraDoors()
{
	// TODO Should probably make this logic more sophisticated since it doesn't scale to the
	//amount of cells neighboring, but it'll do for now
	int32 numberOfRooms = LevelData->GetRows() * LevelData->GetCols();

	int32 numberOfDoorsToAdd = (int32) numberOfRooms * PERCENTAGE_OF_EXTRA_DOORS;

	for (int32 i = 0; i < numberOfDoorsToAdd; i++) {
		//Grab a random cell
		FRoomLocation RandomLocation = GetRandomLocation();

		//Pick a random Direction not given a door
		TArray<Direction> Directions = LevelData->GetDirectionsWithoutDoors(RandomLocation);
		
		if (Directions.Num() == 0) {
			//All doors are filled, so continue
			continue;
		}

		Direction RandomDirection = Directions[rand() % Directions.Num()];
		FRoomLocation NeighborLocation = GetRoomLocationInDirection(RandomLocation, RandomDirection);

		//Give it a door

		LevelData->AddDoorsBetween(RandomLocation, NeighborLocation);
	}
}

FRoomLocation ULevelLayout::GetRandomLocation() const
{
	// Grab random room location in the level
	int Rows = LevelData->GetRows();
	int Cols = LevelData->GetCols();

	int RowToStart = rand() % Rows;
	int ColToStart = rand() % Cols;

	FRoomLocation RoomLocation;
	RoomLocation.Row = RowToStart;
	RoomLocation.Col = ColToStart;

	return RoomLocation;
}

//Returns a RoomLocation of one next to roomLocation that has a door already (there may be multiple and it chooses randomly)
FRoomLocation ULevelLayout::GetNeighborToAttachTo(const FRoomLocation& RoomLocation) const
{
	TArray<FRoomLocation> PossibleAttachPoints = LevelData->GetNeighborsWithDoors(RoomLocation);

	int IndexToSelect = rand() % PossibleAttachPoints.Num();

	return PossibleAttachPoints[IndexToSelect];
}

FRoomLocation ULevelLayout::GetRoomLocationInDirection(const FRoomLocation & RandomLocation, const Direction Direction) const
{
	FRoomLocation Neighbor;
	Neighbor.Row = RandomLocation.Row;
	Neighbor.Col = RandomLocation.Col;

	switch (Direction) {
		case Direction::NORTH:
			Neighbor.Row++;
			break;
		case Direction::SOUTH:
			Neighbor.Row--;
			break;
		case Direction::EAST:
			Neighbor.Col++;
			break;
		case Direction::WEST:
			Neighbor.Col--;
			break;
	}

	return Neighbor;
}
