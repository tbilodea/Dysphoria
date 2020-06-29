// Copyright T Bilodeau 2020

#include "LevelLayout.h"

#include "LevelData.h"
#include "RoomData.h"
#include <vector>
#include <random>
#include <time.h>

LevelLayout::LevelLayout()
{
}

LevelLayout::~LevelLayout()
{
}

void LevelLayout::Setup(const int32 rowRooms, const int32 colRooms)
{
	uint32 rndSeed = time(NULL);
	srand(rndSeed);
	levelData = std::make_shared<LevelData>(LevelData(rowRooms, colRooms, rndSeed));
	levelData->InitializeLevelDataMap();
}

void LevelLayout::Build()
{
	if (!levelData) { return; }

	//Connects all the rooms with doors
	ConnectAllRooms();
	
	//Let's add extra doors to make the map more navigatable and less labyrintian
	AddExtraDoors();

	//Places important rooms
	PlaceEntranceRoom();
	PlaceBossRoom();
}

std::shared_ptr<LevelData> LevelLayout::GetLevelData()
{
	if (!levelData)
	{
		levelData = std::make_shared<LevelData>(LevelData());
		levelData->InitializeLevelDataMap();
		UE_LOG(LogTemp, Warning, TEXT("While getting level data in LevelLayout, the map was not initialized. Returning an empty map."));
	}

	return levelData;
}

// Using a greedy (PRIM) algorithm, connect all the rooms
void LevelLayout::ConnectAllRooms() 
{
	RoomLocation startLocation = GetRandomLocation();
	std::vector<RoomLocation> frontier = {};

	for (auto neighbor : levelData->GetNeighborsWithoutDoors(startLocation)) {
		frontier.emplace_back(neighbor);
	}

	//Looping through the frontier while there is one
	while (frontier.size() != 0) {
		// grab a random frontier room
		RoomLocation selectedFrontierRoom = frontier.at(rand() % frontier.size()); 

		// random bridge to a room already in the map
		RoomLocation inMapNeighbor = GetNeighborToAttachTo(selectedFrontierRoom);

		// add the door to the rooms
		levelData->AddDoorsBetween(inMapNeighbor, selectedFrontierRoom);

		// remove it from the frontier
		frontier.erase(
			std::remove_if(frontier.begin(), frontier.end(), [&](RoomLocation const & loc) {
				return loc.row == selectedFrontierRoom.row && loc.col == selectedFrontierRoom.col;
			}),
			frontier.end());

		// add any neighbor rooms without doors to the frontier (not already assigned)
		for (auto location : levelData->GetNeighborsWithoutDoors(selectedFrontierRoom))
		{
			frontier.emplace_back(location);
		}
	}

}

void LevelLayout::PlaceEntranceRoom() 
{
	// TODO
}

void LevelLayout::PlaceBossRoom()
{
	// TODO
}

void LevelLayout::AddExtraDoors()
{
	// Should probably make this logic more sophisticated since it doesn't scale to the
	//amount of cells neighboring, but it'll do for now
	int32 numberOfRooms = levelData->GetRows() * levelData->GetCols();

	int32 numberOfDoorsToAdd = (int32) numberOfRooms * PERCENTAGE_OF_EXTRA_DOORS;

	for (int32 i = 0; i < numberOfDoorsToAdd; i++) {
		//Grab a random cell
		RoomLocation randomLocation = GetRandomLocation();

		//Pick a random Direction not given a door
		std::vector<DirectionUtils::Direction> directions = levelData->GetDirectionsWithoutDoors(randomLocation);
		
		if (directions.size() == 0) {
			//All doors are filled, so continue
			continue;
		}

		DirectionUtils::Direction randomDirection = directions.at(rand() % directions.size());
		RoomLocation neighborLocation = GetRoomLocationInDirection(randomLocation, randomDirection);

		//Give it a door

		levelData->AddDoorsBetween(randomLocation, neighborLocation);
	}
}

RoomLocation LevelLayout::GetRandomLocation() const
{
	// Grab random room location in the level
	int rows = levelData->GetRows();
	int cols = levelData->GetCols();

	int rowToStart = rand() % rows;
	int colToStart = rand() % cols;

	RoomLocation roomLocation;
	roomLocation.row = rowToStart;
	roomLocation.col = colToStart;

	return roomLocation;
}

//Returns a RoomLocation of one next to roomLocation that has a door already (there may be multiple and it chooses randomly)
RoomLocation LevelLayout::GetNeighborToAttachTo(const RoomLocation& roomLocation) const
{
	std::vector<RoomLocation> possibleAttachPoints = levelData->GetNeighborsWithDoors(roomLocation);

	int indexToSelect = rand() % possibleAttachPoints.size();

	return possibleAttachPoints.at(indexToSelect);
}

RoomLocation LevelLayout::GetRoomLocationInDirection(const RoomLocation & randomLocation, const DirectionUtils::Direction direction) const
{
	RoomLocation neighbor;
	neighbor.row = randomLocation.row;
	neighbor.col = randomLocation.col;

	switch (direction) {
		case DirectionUtils::Direction::NORTH:
			neighbor.row++;
			break;
		case DirectionUtils::Direction::SOUTH:
			neighbor.row--;
			break;
		case DirectionUtils::Direction::EAST:
			neighbor.col++;
			break;
		case DirectionUtils::Direction::WEST:
			neighbor.col--;
			break;
	}

	return neighbor;
}
