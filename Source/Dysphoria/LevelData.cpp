// Copyright T Bilodeau 2020


#include "LevelData.h"

#include "DirectionUtils.h"

//Initialize the map with the appropriate rows/cols of empty RoomDatas
void ULevelData::InitializeLevelDataMap(int32 rows, int32 cols, uint32 seed)
{
	ROWS = rows;
	COLS = cols;
	RND_SEED = seed;

	for (int Row = 0; Row < ROWS; Row++) {
		for (int Col = 0; Col < COLS; Col++) {
			FRoomLocation RoomLocation;
			
			RoomLocation.Row = Row;
			RoomLocation.Col = Col;

			LevelRooms.Add(RoomLocation, NewObject<URoomData>());
		}
	}
}

//Returns levelRooms map
TMap<FRoomLocation, URoomData*> ULevelData::GetAllRoomDatas()
{
	return LevelRooms;
}

//Get the appropriate RoomData for the row col. Defaults to the first room if row/col is out of range.
URoomData* ULevelData::GetRoomData(const int32 Row, const int32 Col)
{
	FRoomLocation Location;
	Location.Row = Row;
	Location.Col = Col;

	auto Room = LevelRooms.Find(Location);
	if (Room) {
		return *Room;
	}
	else 
	{
		Location.Row = 0;
		Location.Col = 0;

		if (LevelRooms.Num() > 0)
			return *LevelRooms.Find(Location);
		else
			return nullptr;
	}
}

TArray<FRoomLocation> ULevelData::GetNeighborsOf(const FRoomLocation & RoomLocation) const
{
	TArray<FRoomLocation> Neighbors;
	
	//Room to the left
	if (RoomLocation.Col - 1 >= 0) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row;
		Neighbor.Col = RoomLocation.Col - 1;
		Neighbors.Add(Neighbor);
	}

	//Room to the right
	if (RoomLocation.Col + 1 < COLS) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row;
		Neighbor.Col = RoomLocation.Col + 1;
		Neighbors.Add(Neighbor);
	}

	//Room above
	if (RoomLocation.Row + 1 < ROWS) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row + 1;
		Neighbor.Col = RoomLocation.Col;
		Neighbors.Add(Neighbor);
	}

	//Room below
	if (RoomLocation.Row - 1 >= 0) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row - 1;
		Neighbor.Col = RoomLocation.Col;
		Neighbors.Add(Neighbor);
	}

	return Neighbors;
}

TArray<FRoomLocation> ULevelData::GetNeighborsWithDoors(const FRoomLocation & RoomLocation) const
{
	TArray<FRoomLocation> Neighbors = GetNeighborsOf(RoomLocation);
	TArray<FRoomLocation> NeighborsWithDoors;

	//Filter out rooms without doors
	for(auto NeighborLocation : Neighbors) {
		auto RoomToCheck = LevelRooms.Find(NeighborLocation);

		if (RoomToCheck != nullptr){
			if (((*RoomToCheck)->ConnectedToAnotherRoom())) {
				NeighborsWithDoors.Add(NeighborLocation);
			}
		}
	}
	
	return NeighborsWithDoors;
}

TArray<FRoomLocation> ULevelData::GetNeighborsWithoutDoors(const FRoomLocation & RoomLocation) const
{
	TArray<FRoomLocation> Neighbors = GetNeighborsOf(RoomLocation);
	TArray<FRoomLocation> NeighborsWithoutDoors;

	//Filter out rooms with doors
	for (auto& NeighborLocation : Neighbors) {
		auto NeighborRoom = LevelRooms.Find(NeighborLocation);

		if (NeighborRoom && !((*NeighborRoom)->ConnectedToAnotherRoom()) ) {
			NeighborsWithoutDoors.Add(NeighborLocation);
		}
	}

	return NeighborsWithoutDoors;
}

TArray<Direction> ULevelData::GetDirectionsWithoutDoors(const FRoomLocation & RoomLocation) const
{
	TArray<Direction> WithoutDoors;
	auto RoomData = LevelRooms.Find(RoomLocation);
	if (!RoomData) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find room location [%i, %i]"), RoomLocation.Row, RoomLocation.Col);
		return WithoutDoors;
	}

	for (auto Direction : DirectionUtils::GetAllDirections()) {
		if (!((*RoomData)->ConnectedToRoom(Direction)) ) {
			WithoutDoors.Add(Direction);
		}
	}

	return WithoutDoors;
}

TArray<FRoomLocation> ULevelData::GetRoomsAround(const FRoomLocation & RoomLocation) const
{
	TArray<FRoomLocation> RoomsAround = GetNeighborsOf(RoomLocation);

	//Room in the NE corner
	if (RoomLocation.Row + 1 < ROWS && RoomLocation.Col + 1 < COLS) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row + 1;
		Neighbor.Col = RoomLocation.Col + 1;
		RoomsAround.Add(Neighbor);
	}
	
	//Room in the NW corner
	if (RoomLocation.Row + 1 < ROWS && RoomLocation.Col - 1 >= 0) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row + 1;
		Neighbor.Col = RoomLocation.Col - 1;
		RoomsAround.Add(Neighbor);
	}

	//Room in the SE corner
	if (RoomLocation.Row - 1 >= 0 && RoomLocation.Col + 1 < COLS) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row - 1;
		Neighbor.Col = RoomLocation.Col + 1;
		RoomsAround.Add(Neighbor);
	}

	//Room in the SW corner
	if (RoomLocation.Row - 1 >= 0 && RoomLocation.Col - 1 >= 0) {
		FRoomLocation Neighbor;
		Neighbor.Row = RoomLocation.Row - 1;
		Neighbor.Col = RoomLocation.Col - 1;
		RoomsAround.Add(Neighbor);
	}

	return RoomsAround;
}

int32 ULevelData::GetRows() const
{
	return ROWS;
}

int32 ULevelData::GetCols() const
{
	return COLS;
}

//Adds the walls to the rooms, TODO there's no error handling here for if they are not next to each other
void ULevelData::AddDoorsBetween(const FRoomLocation & Room1, const FRoomLocation & Room2)
{
	Direction Direction = GetDirectionBetweenRooms(Room1, Room2);

	URoomData** Room1Pointer = LevelRooms.Find(Room1);
	if (!Room1Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), Room1.Row, Room1.Col);
		return;
	}

	URoomData** Room2Pointer = LevelRooms.Find(Room2);
	if (!Room2Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), Room2.Row, Room2.Col);
		return;
	}

	(*Room1Pointer)->AddDoor(Direction);
	(*Room2Pointer)->AddDoor(DirectionUtils::GetOppositeDirection(Direction));
}

void ULevelData::RemoveDoorsBetween(const FRoomLocation & Room1, const FRoomLocation & Room2)
{
	Direction Direction = GetDirectionBetweenRooms(Room1, Room2);

	URoomData** Room1Pointer = LevelRooms.Find(Room1);
	if (!Room1Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i] to remove a door"), Room1.Row, Room1.Col);
		return;
	}

	URoomData** Room2Pointer = LevelRooms.Find(Room2);
	if (!Room2Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i] to remove a door"), Room2.Row, Room2.Col);
		return;
	}

	(*Room1Pointer)->RemoveDoor(Direction);
	(*Room2Pointer)->RemoveDoor(DirectionUtils::GetOppositeDirection(Direction));
}

void ULevelData::SetEntrance(const FRoomLocation Room)
{
	Entrance = Room;
}

void ULevelData::SetBossRoom(const FRoomLocation Room)
{
	BossRoom = Room;
}

//Returns the direction from room1 to room2
Direction ULevelData::GetDirectionBetweenRooms(const FRoomLocation& Room1, const FRoomLocation& Room2) const 
{
	int32 RowDelta = Room1.Row - Room2.Row;
	int32 ColDelta = Room1.Col - Room2.Col;

	if (RowDelta == -1) {
		return Direction::NORTH;
	}

	if (RowDelta == 1) {
		return Direction::SOUTH;
	}

	if (ColDelta == -1) {
		return Direction::EAST;
	}

	if (ColDelta == 1) {
		return Direction::WEST;
	}

	UE_LOG(LogTemp, Warning, TEXT("Room 1 at [%i, %i] and 2 [%i, %i] aren't next to each other, returning a default of NORTH"), Room1.Row, Room1.Col, Room2.Row, Room2.Col);
	return Direction::NORTH;
}