// Copyright T Bilodeau 2020

#include "LevelLayout.h"

#include "LevelData.h"
#include "RoomData.h"
#include <vector>
#include <random>
#include <time.h>
#include <cmath>

void ULevelLayout::Setup(const int32 rowRooms, const int32 colRooms)
{
	uint32 rndSeed = time(NULL);
	srand(rndSeed);
	levelData = NewObject<ULevelData>();
	levelData->InitializeLevelDataMap(rowRooms, colRooms, rndSeed);
}

void ULevelLayout::Build()
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

ULevelData* ULevelLayout::GetLevelData()
{
	if (!levelData)
	{
		levelData = NewObject<ULevelData>();
		levelData->InitializeLevelDataMap(5, 5, 1);
		UE_LOG(LogTemp, Warning, TEXT("While getting level data in ULevelLayout, the map was not initialized. Returning an empty map."));
	}

	return levelData;
}

// Using a greedy (PRIM) algorithm, connect all the rooms
void ULevelLayout::ConnectAllRooms() 
{
	FRoomLocation startLocation = GetRandomLocation();
	std::vector<FRoomLocation> frontier = {};

	for (auto neighbor : levelData->GetNeighborsWithoutDoors(startLocation)) {
		frontier.emplace_back(neighbor);
	}

	//Looping through the frontier while there is one
	while (frontier.size() != 0) {
		// grab a random frontier room
		FRoomLocation selectedFrontierRoom = frontier.at(rand() % frontier.size()); 

		// random bridge to a room already in the map
		FRoomLocation inMapNeighbor = GetNeighborToAttachTo(selectedFrontierRoom);

		// add the door to the rooms
		levelData->AddDoorsBetween(inMapNeighbor, selectedFrontierRoom);

		// remove it from the frontier
		frontier.erase(
			std::remove_if(frontier.begin(), frontier.end(), [&](FRoomLocation const & loc) {
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

//Chosing a first row cell to be the start room
void ULevelLayout::PlaceEntranceRoom() 
{
	FRoomLocation entrance;
	entrance.row = 0;
	entrance.col = rand() % levelData->GetCols();

	levelData->SetEntrance(entrance);
}

//Choosing a last row cell to be the end room, connect every room around it, and then connect it just once randomly to one of those rooms
void ULevelLayout::PlaceBossRoom()
{
	FRoomLocation bossRoom;
	bossRoom.row = levelData->GetRows() - 1;
	bossRoom.col = rand() % levelData->GetCols();

	auto roomsAroundBoss = levelData->GetRoomsAround(bossRoom);

	//Add connections between every room next to each other
	for (auto& roomAroundBoss : roomsAroundBoss) {
		for (auto& aSecondRoom : roomsAroundBoss) {
			if (abs(roomAroundBoss.row - aSecondRoom.row) == 1 || abs(roomAroundBoss.col - aSecondRoom.col) == 1) {
				levelData->AddDoorsBetween(roomAroundBoss, aSecondRoom);
			}
		}
		//Preemptively disconnect the rooms from the boss room
		levelData->RemoveDoorsBetween(bossRoom, roomAroundBoss);
	}

	auto& roomToConnect = roomsAroundBoss.at(rand() % roomsAroundBoss.size());

	levelData->AddDoorsBetween(bossRoom, roomToConnect);
	levelData->SetBossRoom(bossRoom);
}

void ULevelLayout::AddExtraDoors()
{
	// TODO Should probably make this logic more sophisticated since it doesn't scale to the
	//amount of cells neighboring, but it'll do for now
	int32 numberOfRooms = levelData->GetRows() * levelData->GetCols();

	int32 numberOfDoorsToAdd = (int32) numberOfRooms * PERCENTAGE_OF_EXTRA_DOORS;

	for (int32 i = 0; i < numberOfDoorsToAdd; i++) {
		//Grab a random cell
		FRoomLocation randomLocation = GetRandomLocation();

		//Pick a random Direction not given a door
		std::vector<DirectionUtils::Direction> directions = levelData->GetDirectionsWithoutDoors(randomLocation);
		
		if (directions.size() == 0) {
			//All doors are filled, so continue
			continue;
		}

		DirectionUtils::Direction randomDirection = directions.at(rand() % directions.size());
		FRoomLocation neighborLocation = GetRoomLocationInDirection(randomLocation, randomDirection);

		//Give it a door

		levelData->AddDoorsBetween(randomLocation, neighborLocation);
	}
}

FRoomLocation ULevelLayout::GetRandomLocation() const
{
	// Grab random room location in the level
	int rows = levelData->GetRows();
	int cols = levelData->GetCols();

	int rowToStart = rand() % rows;
	int colToStart = rand() % cols;

	FRoomLocation roomLocation;
	roomLocation.row = rowToStart;
	roomLocation.col = colToStart;

	return roomLocation;
}

//Returns a RoomLocation of one next to roomLocation that has a door already (there may be multiple and it chooses randomly)
FRoomLocation ULevelLayout::GetNeighborToAttachTo(const FRoomLocation& roomLocation) const
{
	std::vector<FRoomLocation> possibleAttachPoints = levelData->GetNeighborsWithDoors(roomLocation);

	int indexToSelect = rand() % possibleAttachPoints.size();

	return possibleAttachPoints.at(indexToSelect);
}

FRoomLocation ULevelLayout::GetRoomLocationInDirection(const FRoomLocation & randomLocation, const DirectionUtils::Direction direction) const
{
	FRoomLocation neighbor;
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
