// Copyright T Bilodeau 2020


#include "LevelData.h"

#include "RoomData.h"

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

			auto roomDataPointer = NewObject<URoomData>();

			levelRooms.insert(std::make_pair(roomLocation, roomDataPointer));
		}
	}
}

//Returns levelRooms map
std::map<FRoomLocation, URoomData*> ULevelData::GetAllRoomDatas()
{
	return levelRooms;
}

//Get the appropriate RoomData for the row col. Defaults to the first room if row/col is out of range.
URoomData* ULevelData::GetRoomData(const int row, const int col)
{
	FRoomLocation location;
	location.row = row;
	location.col = col;

	auto it = levelRooms.find(location);
	if (it != levelRooms.end()) {
		return (it->second);
	}
	else 
	{
		location.row = 0;
		location.col = 0;

		auto it2 = levelRooms.find(location);

		return (it2->second);
	}
}

std::vector<FRoomLocation> ULevelData::GetNeighborsOf(const FRoomLocation & roomLocation) const
{
	std::vector<FRoomLocation> neighbors = {};
	
	//Room to the left
	if (roomLocation.col - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row;
		neighbor.col = roomLocation.col - 1;
		neighbors.emplace_back(neighbor);
	}

	//Room to the right
	if (roomLocation.col + 1 < COLS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row;
		neighbor.col = roomLocation.col + 1;
		neighbors.emplace_back(neighbor);
	}

	//Room above
	if (roomLocation.row + 1 < ROWS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row + 1;
		neighbor.col = roomLocation.col;
		neighbors.emplace_back(neighbor);
	}

	//Room below
	if (roomLocation.row - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row - 1;
		neighbor.col = roomLocation.col;
		neighbors.emplace_back(neighbor);
	}

	return neighbors;
}

std::vector<FRoomLocation> ULevelData::GetNeighborsWithDoors(const FRoomLocation & roomLocation) const
{
	std::vector<FRoomLocation> neighbors = GetNeighborsOf(roomLocation);
	std::vector<FRoomLocation> neighborsWithDoors = {};

	//Filter out rooms without doors
	for(auto neighborLocation : neighbors) {
		auto it = levelRooms.find(neighborLocation);

		if (it != levelRooms.end()){
			
			auto roomToCheck = it->second;
			if ((roomToCheck->ConnectedToAnotherRoom())) {
				neighborsWithDoors.emplace_back(neighborLocation);
			}
		}
	}
	
	return neighborsWithDoors;
}

std::vector<FRoomLocation> ULevelData::GetNeighborsWithoutDoors(const FRoomLocation & roomLocation) const
{
	std::vector<FRoomLocation> neighbors = GetNeighborsOf(roomLocation);
	std::vector<FRoomLocation> neighborsWithoutDoors = {};

	//Filter out rooms with doors
	for (auto neighborLocation : neighbors) {
		auto it = levelRooms.find(neighborLocation);

		if ( it != levelRooms.end() && !((it->second)->ConnectedToAnotherRoom()) ) {
			neighborsWithoutDoors.emplace_back(neighborLocation);
		}
	}

	return neighborsWithoutDoors;
}

std::vector<DirectionUtils::Direction> ULevelData::GetDirectionsWithoutDoors(const FRoomLocation & roomLocation) const
{
	std::vector<DirectionUtils::Direction> withoutDoors = {};
	auto it = levelRooms.find(roomLocation);
	if (it == levelRooms.end()) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find room location [%i, %i]"), roomLocation.row, roomLocation.col);
		return withoutDoors;
	}
	auto roomData = it->second;
	for (auto direction : DirectionUtils::GetAllDirections()) {
		if ( !(roomData->ConnectedToRoom(direction)) ) {
			withoutDoors.emplace_back(direction);
		}
	}

	return withoutDoors;
}

std::vector<FRoomLocation> ULevelData::GetRoomsAround(const FRoomLocation & roomLocation) const
{
	std::vector<FRoomLocation> roomsAround = GetNeighborsOf(roomLocation);

	//Room in the NE corner
	if (roomLocation.row + 1 < ROWS && roomLocation.col + 1 < COLS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row + 1;
		neighbor.col = roomLocation.col + 1;
		roomsAround.emplace_back(neighbor);
	}
	
	//Room in the NW corner
	if (roomLocation.row + 1 < ROWS && roomLocation.col - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row + 1;
		neighbor.col = roomLocation.col - 1;
		roomsAround.emplace_back(neighbor);
	}

	//Room in the SE corner
	if (roomLocation.row - 1 >= 0 && roomLocation.col + 1 < COLS) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row - 1;
		neighbor.col = roomLocation.col + 1;
		roomsAround.emplace_back(neighbor);
	}

	//Room in the SW corner
	if (roomLocation.row - 1 >= 0 && roomLocation.col - 1 >= 0) {
		FRoomLocation neighbor;
		neighbor.row = roomLocation.row - 1;
		neighbor.col = roomLocation.col - 1;
		roomsAround.emplace_back(neighbor);
	}

	return roomsAround;
}

int ULevelData::GetRows() const
{
	return ROWS;
}

int ULevelData::GetCols() const
{
	return COLS;
}

//Adds the walls to the rooms, TODO there's no error handling here for if they are not next to each other
void ULevelData::AddDoorsBetween(const FRoomLocation & room1, const FRoomLocation & room2)
{
	DirectionUtils::Direction direction = GetDirectionBetweenRooms(room1, room2);

	URoomData* roomData1;

	auto it1 = levelRooms.find(room1);
	if (it1 != levelRooms.end()) {
		roomData1 = it1->second;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), room1.row, room1.col);
		return;
	}

	URoomData* roomData2;

	auto it2 = levelRooms.find(room2);
	if (it2 != levelRooms.end()) {
		roomData2 = it2->second;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), room2.row, room2.col);
		return;
	}

	roomData1->AddDoor(direction);
	roomData2->AddDoor(DirectionUtils::GetOppositeDirection(direction));
}

void ULevelData::RemoveDoorsBetween(const FRoomLocation & room1, const FRoomLocation & room2)
{
	DirectionUtils::Direction direction = GetDirectionBetweenRooms(room1, room2);

	URoomData* roomData1;

	auto it1 = levelRooms.find(room1);
	if (it1 != levelRooms.end()) {
		roomData1 = it1->second;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), room1.row, room1.col);
		return;
	}

	URoomData* roomData2;

	auto it2 = levelRooms.find(room2);
	if (it2 != levelRooms.end()) {
		roomData2 = it2->second;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), room2.row, room2.col);
		return;
	}

	roomData1->RemoveDoor(direction);
	roomData2->RemoveDoor(DirectionUtils::GetOppositeDirection(direction));
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
DirectionUtils::Direction ULevelData::GetDirectionBetweenRooms(const FRoomLocation& room1, const FRoomLocation& room2) const 
{
	int32 rowDelta = room1.row - room2.row;
	int32 colDelta = room1.col - room2.col;

	if (rowDelta == -1) {
		return DirectionUtils::Direction::NORTH;
	}

	if (rowDelta == 1) {
		return DirectionUtils::Direction::SOUTH;
	}

	if (colDelta == -1) {
		return DirectionUtils::Direction::EAST;
	}

	if (colDelta == 1) {
		return DirectionUtils::Direction::WEST;
	}

	UE_LOG(LogTemp, Warning, TEXT("Room 1 at [%i, %i] and 2 [%i, %i] aren't next to each other, returning a default of NORTH"), room1.row, room1.col, room2.row, room2.col);
	return DirectionUtils::Direction::NORTH;
}