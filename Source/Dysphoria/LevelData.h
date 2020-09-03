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

	int32 Row;
	int32 Col;

	bool operator==(const FRoomLocation& Rhs) const {
		return Row == Rhs.Row && Col == Rhs.Col;
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
	URoomData* GetRoomData(const int32 Row, const int32 Col);

	void InitializeLevelDataMap(int32 Rows, int32 Cols, uint32 Seed);

	TArray<FRoomLocation> GetNeighborsOf(const FRoomLocation& RoomLocation) const;

	TArray<FRoomLocation> GetNeighborsWithDoors(const FRoomLocation& RoomLocation) const;

	TArray<FRoomLocation> GetNeighborsWithoutDoors(const FRoomLocation& RoomLocation) const;

	TArray<Direction> GetDirectionsWithoutDoors(const FRoomLocation& RoomLocation) const;

	TArray<FRoomLocation> GetRoomsAround(const FRoomLocation& RoomLocation) const;

	int32 GetRows() const;
	int32 GetCols() const;

	//Methods to manipulate underlying data
	void AddDoorsBetween(const FRoomLocation& Room1, const FRoomLocation& Room2);
	void RemoveDoorsBetween(const FRoomLocation& Room1, const FRoomLocation& Room2);
	void SetEntrance(const FRoomLocation Room);
	void SetBossRoom(const FRoomLocation Room);

private:
	int32 ROWS = 5;
	int32 COLS = 5;
	uint32 RND_SEED = 1;
	FRoomLocation Entrance;
	FRoomLocation BossRoom;

	TMap<FRoomLocation, URoomData*> LevelRooms;

	Direction GetDirectionBetweenRooms(const FRoomLocation& Room1, const FRoomLocation& Room2) const;
};
