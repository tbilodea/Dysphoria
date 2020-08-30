// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include "DirectionUtils.h"
#include <memory>
#include <map>

#include "LevelData.generated.h"

class URoomData;

/*
* Struct for defining each rooms location, since this is data that doesn't need to be in the RoomData
*/
USTRUCT()
struct FRoomLocation {
	GENERATED_BODY()

	int32 row;
	int32 col;

	bool operator==(const FRoomLocation& rhs) const {
		return row == rhs.row && col == rhs.col;
	}

	bool operator>(const FRoomLocation& rhs) const {
		if (row != rhs.row) {
			if (row > rhs.row) {
				return true;
			}
			else if (row < rhs.row) {
				return false;
			}
		}
		//rows are equal, so use columns
		return col > rhs.col;
	}

	bool operator<(const FRoomLocation& rhs) const {
		if (row != rhs.row) {
			if (row < rhs.row) {
				return true;
			}
			else if (row > rhs.row) {
				return false;
			}
		}
		//rows are equal, so use columns
		return col < rhs.col;
	}
};

/**
 * LevelData exists to store the data about a level
 *
 * Row/Col definitions of the RoomLocation are based in Quadrant I ([0,0] being the bottomleft or SOUTH-WEST corner)
 * 
 */
UCLASS()
class DYSPHORIA_API ULevelData : public UObject
{
	GENERATED_BODY()
public:

	//Returns copy of levelRooms
	std::map<FRoomLocation, URoomData*> GetAllRoomDatas();

	//Returns a pointer to the RoomData at row/col
	URoomData* GetRoomData(const int row, const int col);

	void InitializeLevelDataMap(int32 rows, int32 cols, uint32 seed);

	std::vector<FRoomLocation> GetNeighborsOf(const FRoomLocation& roomLocation) const;

	std::vector<FRoomLocation> GetNeighborsWithDoors(const FRoomLocation& roomLocation) const;

	std::vector<FRoomLocation> GetNeighborsWithoutDoors(const FRoomLocation& roomLocation) const;

	std::vector<DirectionUtils::Direction> GetDirectionsWithoutDoors(const FRoomLocation& roomLocation) const;

	std::vector<FRoomLocation> GetRoomsAround(const FRoomLocation& roomLocation) const;

	int GetRows() const;
	int GetCols() const;

	//Methods to manipulate underlying data
	void AddDoorsBetween(const FRoomLocation& room1, const FRoomLocation& room2);
	void RemoveDoorsBetween(const FRoomLocation& room1, const FRoomLocation& room2);
	void SetEntrance(const FRoomLocation room);
	void SetBossRoom(const FRoomLocation room);

private:
	int32 ROWS = 5;
	int32 COLS = 5;
	uint32 RND_SEED = 1;
	FRoomLocation entrance;
	FRoomLocation bossRoom;

	std::map<FRoomLocation, URoomData*> levelRooms;

	DirectionUtils::Direction GetDirectionBetweenRooms(const FRoomLocation& room1, const FRoomLocation& room2) const;
};
