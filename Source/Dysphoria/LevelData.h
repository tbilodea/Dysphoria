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
 * This class is only Blueprintable and has UFUNCTIONS for unit testing purposes
 */
UCLASS(Blueprintable)
class DYSPHORIA_API ULevelData : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void InitializeLevelDataMap(int32 Rows, int32 Cols, int32 Seed);

	//Returns copy of levelRooms
	TMap<FRoomLocation, URoomData*> GetAllRoomDatas();

	//Returns a pointer to the RoomData at row/col
	UFUNCTION(BlueprintCallable)
	URoomData* GetRoomData(const int32 Row, const int32 Col);

	TArray<FRoomLocation> GetNeighborsOf(const FRoomLocation& RoomLocation) const;
	TArray<FRoomLocation> GetNeighborsWithDoors(const FRoomLocation& RoomLocation) const;
	TArray<FRoomLocation> GetNeighborsWithoutDoors(const FRoomLocation& RoomLocation) const;
	TArray<Direction> GetDirectionsWithoutDoors(const FRoomLocation& RoomLocation) const;
	TArray<FRoomLocation> GetRoomsAround(const FRoomLocation& RoomLocation) const;

	//Grabs the Directions that there are neighbors (if it exists on the edge of the map, not all directions will be present)
	TArray<Direction> GetNeighborDirections(const FRoomLocation & RoomLocation) const;

	UFUNCTION(BlueprintCallable)
	int32 GetRows() const;
	UFUNCTION(BlueprintCallable)
	int32 GetCols() const;

	FRoomLocation GetBossRoom() const;
	FRoomLocation GetEntrance() const;

	UFUNCTION(BlueprintCallable)
	URowCol* GetBossRoomLocation();
	UFUNCTION(BlueprintCallable)
	URowCol* GetEntranceLocation();

	//Methods to manipulate underlying data
	void AddDoorsBetween(const FRoomLocation& Room1, const FRoomLocation& Room2);
	void RemoveDoorsBetween(const FRoomLocation& Room1, const FRoomLocation& Room2);

	void SetEntrance(const FRoomLocation Room);
	void SetBossRoom(const FRoomLocation Room);

protected:
	//TEST FUNCTIONS
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetNeighborsOf(int32 Row, int32 Col);
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetNeighborsWithDoors(int32 Row, int32 Col);
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetNeighborsWithoutDoors(int32 Row, int32 Col);
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetDirectionsWithoutDoors(int32 Row, int32 Col);
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetRoomsAround(int32 Row, int32 Col);
	UFUNCTION(BlueprintCallable)
	void TESTBP_AddDoorsBetween(int32 Row, int32 Col, int32 Row2, int32 Col2);
	UFUNCTION(BlueprintCallable)
	void TESTBP_RemoveDoorsBetween(int32 Row, int32 Col, int32 Row2, int32 Col2);

private:
	int32 ROWS = 5;
	int32 COLS = 5;
	int32 RND_SEED = 1;
	FRoomLocation Entrance;
	FRoomLocation BossRoom;

	TMap<FRoomLocation, URoomData*> LevelRooms;

	Direction GetDirectionBetweenRooms(const FRoomLocation& Room1, const FRoomLocation& Room2) const;
	URowCol* CreateRowCol(const FRoomLocation RoomLoc);
};
