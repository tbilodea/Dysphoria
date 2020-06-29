// Copyright T Bilodeau 2020


#include "LevelData.h"
#include "RoomData.h"

LevelData::LevelData() : ROWS(5), COLS(5), RND_SEED(-1)
{
	InitializeLevelDataMap();
}

LevelData::LevelData(int32 rows, int32 cols, uint32 seed) : ROWS(rows), COLS(cols), RND_SEED(seed)
{
	InitializeLevelDataMap();
}

LevelData::~LevelData()
{
}

//Initialize the map with the appropriate rows/cols of empty RoomDatas
void LevelData::InitializeLevelDataMap()
{
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			RoomLocation roomLocation;
			
			roomLocation.row = row;
			roomLocation.col = col;

			auto roomDataPointer = std::make_shared<RoomData>(RoomData());

			levelRooms.insert(std::make_pair(roomLocation, roomDataPointer));
		}
	}
}

//Returns copy of levelRooms map
std::map<RoomLocation, std::shared_ptr<RoomData>> LevelData::GetAllRoomDatas()
{
	return std::map<RoomLocation, std::shared_ptr<RoomData>>(levelRooms);
}

//Get the appropriate RoomData for the row col. Defaults to the first room if row/col is out of range.
std::shared_ptr<RoomData> LevelData::GetRoomData(const int row, const int col)
{
	RoomLocation location;
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

std::vector<RoomLocation> LevelData::GetNeighborsOf(const RoomLocation & roomLocation) const
{
	std::vector<RoomLocation> neighbors = {};
	
	//Room to the left
	if (roomLocation.col - 1 >= 0) {
		RoomLocation neighbor;
		neighbor.row = roomLocation.row;
		neighbor.col = roomLocation.col - 1;
		neighbors.emplace_back(neighbor);
	}

	//Room to the right
	if (roomLocation.col + 1 < COLS) {
		RoomLocation neighbor;
		neighbor.row = roomLocation.row;
		neighbor.col = roomLocation.col + 1;
		neighbors.emplace_back(neighbor);
	}

	//Room above
	if (roomLocation.row + 1 < ROWS) {
		RoomLocation neighbor;
		neighbor.row = roomLocation.row + 1;
		neighbor.col = roomLocation.col;
		neighbors.emplace_back(neighbor);
	}

	//Room below
	if (roomLocation.row - 1 >= 0) {
		RoomLocation neighbor;
		neighbor.row = roomLocation.row - 1;
		neighbor.col = roomLocation.col;
		neighbors.emplace_back(neighbor);
	}

	return neighbors;
}

std::vector<RoomLocation> LevelData::GetNeighborsWithDoors(const RoomLocation & roomLocation) const
{
	std::vector<RoomLocation> neighbors = GetNeighborsOf(roomLocation);
	std::vector<RoomLocation> neighborsWithDoors = {};

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

std::vector<RoomLocation> LevelData::GetNeighborsWithoutDoors(const RoomLocation & roomLocation) const
{
	std::vector<RoomLocation> neighbors = GetNeighborsOf(roomLocation);
	std::vector<RoomLocation> neighborsWithoutDoors = {};

	//Filter out rooms with doors
	for (auto neighborLocation : neighbors) {
		auto it = levelRooms.find(neighborLocation);

		if ( it != levelRooms.end() && !((it->second)->ConnectedToAnotherRoom()) ) {
			neighborsWithoutDoors.emplace_back(neighborLocation);
		}
	}

	return neighborsWithoutDoors;
}

int LevelData::GetRows() const
{
	return ROWS;
}

int LevelData::GetCols() const
{
	return COLS;
}

//Adds the walls to the rooms, TODO there's no error handling here for if they are not next to each other
void LevelData::AddDoorsBetween(const RoomLocation & room1, const RoomLocation & room2)
{
	DirectionUtils::Direction direction = GetDirectionBetweenRooms(room1, room2);

	std::shared_ptr<RoomData> roomData1;

	auto it1 = levelRooms.find(room1);
	if (it1 != levelRooms.end()) {
		roomData1 = it1->second;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to find room at room location [%i, %i]"), room1.row, room1.col);
		return;
	}

	std::shared_ptr<RoomData> roomData2;

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

//Returns the direction from room1 to room2
DirectionUtils::Direction LevelData::GetDirectionBetweenRooms(const RoomLocation& room1, const RoomLocation& room2) const 
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