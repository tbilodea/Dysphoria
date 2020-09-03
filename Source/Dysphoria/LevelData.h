// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include "Directions.h"
#include "RoomData.h"

#include "LevelData.generated.h"

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
};

//For FRoomLocation use in maps as a key
FORCEINLINE uint32 GetTypeHash(const FRoomLocation& loc) {
	return FCrc::MemCrc32(&loc, sizeof(FRoomLocation));
}

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
	TMap<FRoomLocation, URoomData*> GetAllRoomDatas();

	//Returns a pointer to the RoomData at row/col
	URoomData* GetRoomData(const int32 row, const int32 col);

	void InitializeLevelDataMap(int32 rows, int32 cols, uint32 seed);

	TArray<FRoomLocation> GetNeighborsOf(const FRoomLocation& roomLocation) const;

	TArray<FRoomLocation> GetNeighborsWithDoors(const FRoomLocation& roomLocation) const;

	TArray<FRoomLocation> GetNeighborsWithoutDoors(const FRoomLocation& roomLocation) const;

	TArray<Direction> GetDirectionsWithoutDoors(const FRoomLocation& roomLocation) const;

	TArray<FRoomLocation> GetRoomsAround(const FRoomLocation& roomLocation) const;

	int32 GetRows() const;
	int32 GetCols() const;

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

	TMap<FRoomLocation, URoomData*> LevelRooms;

	Direction GetDirectionBetweenRooms(const FRoomLocation& room1, const FRoomLocation& room2) const;
};
