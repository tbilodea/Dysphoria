// Copyright T Bilodeau 2020


#include "LevelData.h"

#include "DirectionUtils.h"

//Initialize the map with the appropriate rows/cols of empty RoomDatas
void ULevelData::InitializeLevelDataMap(int32 rows, int32 cols, uint32 seed)
{
	ROWS = rows;
	COLS = cols;
	RND_SEED = seed;

	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			FRoomLocation roomLocation;
			
			roomLocation.row = row;
			roomLocation.col = col;

			LevelRooms.Add(roomLocation, NewObject<URoomData>());
		}
	}
}

//Returns levelRooms map
TMap<FRoomLocation, URoomData*> ULevelData::GetAllRoomDatas()
{
	return LevelRooms;
}

//Get the appropriate RoomData for the row col. Defaults to the first room if row/col is out of range.
URoomData* ULevelData::GetRoomData(const int32 row, const int32 col)
{
	FRoomLocation location;
	location.row = row;
	location.col = col;

	auto Room = LevelRooms.Find(location);
	if (Room) {
		return *Room;
	}
	else 
	{
		location.row = 0;
		location.col = 0;

		if (LevelRooms.Num() > 0)
			return *LevelRooms.Find(location);
		else
			return nullptr;
	}
}

TArray<FRoomLocation> ULevelData::GetNeighborsOf(const FRoomLocation & roomLocation) const
{
	TArray<FRoomLocation> neighbors;
	
	//Room to the left
	if (roomLocation.col - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row;
		neighbor.col = roomLocation.col - 1;
		neighbors.Add(neighbor);
	}

	//Room to the right
	if (roomLocation.col + 1 < COLS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row;
		neighbor.col = roomLocation.col + 1;
		neighbors.Add(neighbor);
	}

	//Room above
	if (roomLocation.row + 1 < ROWS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row + 1;
		neighbor.col = roomLocation.col;
		neighbors.Add(neighbor);
	}

	//Room below
	if (roomLocation.row - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row - 1;
		neighbor.col = roomLocation.col;
		neighbors.Add(neighbor);
	}

	return neighbors;
}

TArray<FRoomLocation> ULevelData::GetNeighborsWithDoors(const FRoomLocation & roomLocation) const
{
	TArray<FRoomLocation> neighbors = GetNeighborsOf(roomLocation);
	TArray<FRoomLocation> neighborsWithDoors;

	//Filter out rooms without doors
	for(auto neighborLocation : neighbors) {
		auto roomToCheck = LevelRooms.Find(neighborLocation);

		if (roomToCheck != nullptr){
			if (((*roomToCheck)->ConnectedToAnotherRoom())) {
				neighborsWithDoors.Add(neighborLocation);
			}
		}
	}
	
	return neighborsWithDoors;
}

TArray<FRoomLocation> ULevelData::GetNeighborsWithoutDoors(const FRoomLocation & roomLocation) const
{
	TArray<FRoomLocation> neighbors = GetNeighborsOf(roomLocation);
	TArray<FRoomLocation> neighborsWithoutDoors;

	//Filter out rooms with doors
	for (auto& neighborLocation : neighbors) {
		auto neighborRoom = LevelRooms.Find(neighborLocation);

		if (neighborRoom && !((*neighborRoom)->ConnectedToAnotherRoom()) ) {
			neighborsWithoutDoors.Add(neighborLocation);
		}
	}

	return neighborsWithoutDoors;
}

TArray<Direction> ULevelData::GetDirectionsWithoutDoors(const FRoomLocation & roomLocation) const
{
	TArray<Direction> withoutDoors;
	auto RoomData = LevelRooms.Find(roomLocation);
	if (!RoomData) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find room location [%i, %i]"), roomLocation.row, roomLocation.col);
		return withoutDoors;
	}

	for (auto direction : DirectionUtils::GetAllDirections()) {
		if (!((*RoomData)->ConnectedToRoom(direction)) ) {
			withoutDoors.Add(direction);
		}
	}

	return withoutDoors;
}

TArray<FRoomLocation> ULevelData::GetRoomsAround(const FRoomLocation & roomLocation) const
{
	TArray<FRoomLocation> roomsAround = GetNeighborsOf(roomLocation);

	//Room in the NE corner
	if (roomLocation.row + 1 < ROWS && roomLocation.col + 1 < COLS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row + 1;
		neighbor.col = roomLocation.col + 1;
		roomsAround.Add(neighbor);
	}
	
	//Room in the NW corner
	if (roomLocation.row + 1 < ROWS && roomLocation.col - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row + 1;
		neighbor.col = roomLocation.col - 1;
		roomsAround.Add(neighbor);
	}

	//Room in the SE corner
	if (roomLocation.row - 1 >= 0 && roomLocation.col + 1 < COLS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row - 1;
		neighbor.col = roomLocation.col + 1;
		roomsAround.Add(neighbor);
	}

	//Room in the SW corner
	if (roomLocation.row - 1 >= 0 && roomLocation.col - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row - 1;
		neighbor.col = roomLocation.col - 1;
		roomsAround.Add(neighbor);
	}

	return roomsAround;
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
void ULevelData::AddDoorsBetween(const FRoomLocation & room1, const FRoomLocation & room2)
{
	Direction direction = GetDirectionBetweenRooms(room1, room2);

	URoomData** Room1Pointer = LevelRooms.Find(room1);
	if (!Room1Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), room1.row, room1.col);
		return;
	}

	URoomData** Room2Pointer = LevelRooms.Find(room2);
	if (!Room2Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), room2.row, room2.col);
		return;
	}

	(*Room1Pointer)->AddDoor(direction);
	(*Room2Pointer)->AddDoor(DirectionUtils::GetOppositeDirection(direction));
}

void ULevelData::RemoveDoorsBetween(const FRoomLocation & room1, const FRoomLocation & room2)
{
	Direction direction = GetDirectionBetweenRooms(room1, room2);

	URoomData** Room1Pointer = LevelRooms.Find(room1);
	if (!Room1Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i] to remove a door"), room1.row, room1.col);
		return;
	}

	URoomData** Room2Pointer = LevelRooms.Find(room2);
	if (!Room2Pointer) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i] to remove a door"), room2.row, room2.col);
		return;
	}

	(*Room1Pointer)->RemoveDoor(direction);
	(*Room2Pointer)->RemoveDoor(DirectionUtils::GetOppositeDirection(direction));
}

void ULevelData::SetEntrance(const FRoomLocation room)
{
	entrance = room;
}

void ULevelData::SetBossRoom(const FRoomLocation room)
{
	bossRoom = room;
}

//Returns the direction from room1 to room2
Direction ULevelData::GetDirectionBetweenRooms(const FRoomLocation& room1, const FRoomLocation& room2) const 
{
	int32 rowDelta = room1.row - room2.row;
	int32 colDelta = room1.col - room2.col;

	if (rowDelta == -1) {
		return Direction::NORTH;
	}

	if (rowDelta == 1) {
		return Direction::SOUTH;
	}

	if (colDelta == -1) {
		return Direction::EAST;
	}

	if (colDelta == 1) {
		return Direction::WEST;
	}

	UE_LOG(LogTemp, Warning, TEXT("Room 1 at [%i, %i] and 2 [%i, %i] aren't next to each other, returning a default of NORTH"), room1.row, room1.col, room2.row, room2.col);
	return Direction::NORTH;
}