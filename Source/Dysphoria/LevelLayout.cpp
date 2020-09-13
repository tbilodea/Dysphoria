// Copyright T Bilodeau 2020

#include "LevelLayout.h"

#include "LevelData.h"
#include "RoomData.h"

#include <cmath>
#include "Dysphoria.h"

void ULevelLayout::Setup(const int32 RowRooms, const int32 ColRooms)
{
	RandomStream.GenerateNewSeed();
	int32 RndSeed = RandomStream.GetCurrentSeed();
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

	UE_LOG(LogLevelGeneration, Log, TEXT("Finished Level Layout Build"));
}

ULevelData* ULevelLayout::GetLevelData()
{
	if (!LevelData)
	{
		LevelData = NewObject<ULevelData>();
		LevelData->InitializeLevelDataMap(5, 5, 1);
		UE_LOG(LogLevelGeneration, Warning, TEXT("While getting level data in ULevelLayout, the map was not initialized. Returning an empty map."));
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

	bool FirstPass = true;
	//Looping through the frontier while there is one
	while (Frontier.Num() > 0) {
		// grab a random frontier room
		FRoomLocation SelectedFrontierRoom = Frontier[RandomStream.RandRange(0, Frontier.Num()-1)]; 

		// Bridge randomly to a room already in the map (has doors)
		FRoomLocation InMapNeighbor = GetNeighborToAttachTo(SelectedFrontierRoom);

		//First time through, the Selected Room won't find anything with doors yet since nothing has doors,
		// so we'll handle that pass by attaching it to the Starting Room
		if (FirstPass)
		{
			FirstPass = false;
			InMapNeighbor = StartLocation;
		}


		if (InMapNeighbor.Row == -1) {
			UE_LOG(LogLevelGeneration, Warning, TEXT("Could not find a neighbor to attach for [%i, %i], removing it from the frontier list."), SelectedFrontierRoom.Row, SelectedFrontierRoom.Col);
			Frontier.Remove(SelectedFrontierRoom);
			continue;
		}

		// add the door to the rooms
		LevelData->AddDoorsBetween(InMapNeighbor, SelectedFrontierRoom);

		// remove it from the frontier
		Frontier.Remove(SelectedFrontierRoom);

		// add any neighbor rooms without doors to the frontier (not already assigned)
		Frontier.Append(LevelData->GetNeighborsWithoutDoors(SelectedFrontierRoom));
	}
	UE_LOG(LogLevelGeneration, Log, TEXT("Finished Connecting Rooms"));
}

//Chosing a first row cell to be the start room
void ULevelLayout::PlaceEntranceRoom() 
{
	FRoomLocation Entrance;
	Entrance.Row = 0;
	Entrance.Col = RandomStream.RandRange(0, LevelData->GetCols() - 1);

	LevelData->SetEntrance(Entrance);
}

//Choosing a last row cell to be the end room, connect every room around it, and then connect it just once randomly to one of those rooms
void ULevelLayout::PlaceBossRoom()
{
	FRoomLocation BossRoom;
	BossRoom.Row = LevelData->GetRows() - 1;
	BossRoom.Col = RandomStream.RandRange(0, LevelData->GetCols() - 1);
		
	auto RoomsAroundBoss = LevelData->GetRoomsAround(BossRoom);

	//Add connections between every room next to each other
	for (auto& RoomAroundBoss : RoomsAroundBoss) {
		for (auto& ASecondRoom : RoomsAroundBoss) {
			if (( abs(RoomAroundBoss.Row - ASecondRoom.Row) == 1 
				 && RoomAroundBoss.Col == ASecondRoom.Col )
				|| ( RoomAroundBoss.Row == ASecondRoom.Row
				 && abs(RoomAroundBoss.Col - ASecondRoom.Col) == 1 )) {
				LevelData->AddDoorsBetween(RoomAroundBoss, ASecondRoom);
				UE_LOG(LogLevelGeneration, Log, TEXT("Connecting [%i, %i] and [%i, %i]"), RoomAroundBoss.Row, RoomAroundBoss.Col, ASecondRoom.Row, ASecondRoom.Col);
			}
		}
		//Disconnect all the adjacent rooms from the boss room
		if (abs(RoomAroundBoss.Row - BossRoom.Row) + abs(BossRoom.Col - RoomAroundBoss.Col) == 1) {
			UE_LOG(LogLevelGeneration, Log, TEXT("Removing connection between boss room and [%i, %i]"), RoomAroundBoss.Row, RoomAroundBoss.Col);
			LevelData->RemoveDoorsBetween(BossRoom, RoomAroundBoss);
		}
	}

	auto& RoomToConnect = RoomsAroundBoss[RandomStream.RandRange(0, RoomsAroundBoss.Num() - 1)];

	UE_LOG(LogLevelGeneration, Log, TEXT("Boss Room connected to [%i, %i]"), RoomToConnect.Row, RoomToConnect.Col);

	LevelData->AddDoorsBetween(BossRoom, RoomToConnect);
	LevelData->SetBossRoom(BossRoom);

	UE_LOG(LogLevelGeneration, Log, TEXT("Finished Setting Boss Room"));
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

		Direction RandomDirection = Directions[RandomStream.RandRange(0, Directions.Num() -1 )];
		FRoomLocation NeighborLocation = GetRoomLocationInDirection(RandomLocation, RandomDirection);

		//Give it a door

		LevelData->AddDoorsBetween(RandomLocation, NeighborLocation);
	}

	UE_LOG(LogLevelGeneration, Log, TEXT("Finished Adding Extra Room Connections"));
}

FRoomLocation ULevelLayout::GetRandomLocation() const
{
	// Grab random room location in the level
	int32 Rows = LevelData->GetRows();
	int32 Cols = LevelData->GetCols();

	int32 RowToStart = RandomStream.RandRange(0, Rows - 1);
	int32 ColToStart = RandomStream.RandRange(0, Cols - 1);

	FRoomLocation RoomLocation;
	RoomLocation.Row = RowToStart;
	RoomLocation.Col = ColToStart;

	return RoomLocation;
}

//Returns a RoomLocation of one next to roomLocation that has a door already (there may be multiple and it chooses randomly)
FRoomLocation ULevelLayout::GetNeighborToAttachTo(const FRoomLocation& RoomLocation) const
{
	TArray<FRoomLocation> PossibleAttachPoints = LevelData->GetNeighborsWithDoors(RoomLocation);

	if (PossibleAttachPoints.Num() == 0) {
		FRoomLocation BadLocation;
		BadLocation.Row = -1;
		BadLocation.Col = -1;
		return BadLocation;
	}

	int32 IndexToSelect = RandomStream.RandRange(0, PossibleAttachPoints.Num() - 1 );

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

//TEST METHODS
TArray<int32> ULevelLayout::TESTBP_GetNeighborToAttachTo(int32 Row, int32 Col) const
{
	TArray<int32> Location;
	FRoomLocation Room;
	Room.Row = Row;
	Room.Col = Col;
	FRoomLocation NeighborRoom = GetNeighborToAttachTo(Room);
	Location.Add(NeighborRoom.Row);
	Location.Add(NeighborRoom.Col);
	return Location;
}

TArray<int32> ULevelLayout::TESTBP_GetRoomLocationInDirection(int32 Row, int32 Col, Direction Direction) const
{
	TArray<int32> Location;
	FRoomLocation Room;
	Room.Row = Row;
	Room.Col = Col;
	FRoomLocation NeighborRoom = GetRoomLocationInDirection(Room, Direction);
	Location.Add(NeighborRoom.Row);
	Location.Add(NeighborRoom.Col);
	return Location;
}

TArray<int32> ULevelLayout::TESTBP_GetRandomDirection() const 
{
	TArray<int32> RandomChoice;
	FRoomLocation RoomLocation = GetRandomLocation();
	RandomChoice.Add(RoomLocation.Row);
	RandomChoice.Add(RoomLocation.Col);
	return RandomChoice;
}

void ULevelLayout::TESTBP_SetLevelData(ULevelData* ALevelData) 
{
	LevelData = ALevelData;
}