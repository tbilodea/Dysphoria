// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include "Directions.h"
#include "Math/RandomStream.h"

#include "LevelLayout.generated.h"


class ULevelData;
struct FRoomLocation;

/**
 * Class that builds out the LevelData
 */
UCLASS(Blueprintable)
class DYSPHORIA_API ULevelLayout : public UObject
{
	GENERATED_BODY()

public:
	void Setup(const int32 RowRooms, const int32 ColRooms);

	//Ensure Setup is called before Build
	void Build();

	//Ensure Build was called before this method
	ULevelData* GetLevelData();

	//TEST METHODS
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetNeighborToAttachTo(int32 Row, int32 Col) const;
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetRoomLocationInDirection(int32 Row, int32 Col, Direction Direction) const;
	UFUNCTION(BlueprintCallable)
	TArray<int32> TESTBP_GetRandomDirection() const;
	UFUNCTION(BlueprintCallable)
	void TESTBP_SetLevelData(ULevelData* LevelData);

private:
	float PERCENTAGE_OF_EXTRA_DOORS = 0.33f;
	FRandomStream RandomStream;

	ULevelData* LevelData = nullptr;

	//Map building methods
	void ConnectAllRooms();
	void PlaceEntranceRoom();
	void PlaceBossRoom();
	void AddExtraDoors();

	FRoomLocation GetRandomLocation() const;
	FRoomLocation GetNeighborToAttachTo(const FRoomLocation& RoomLocation) const;
	FRoomLocation GetRoomLocationInDirection(const FRoomLocation& RandomLocation, const Direction Direction) const;
};
