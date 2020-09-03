// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include "Directions.h"

#include <memory>
#include "LevelLayout.generated.h"


class ULevelData;
struct FRoomLocation;

/**
 * Class that builds out the LevelData
 */
UCLASS()
class DYSPHORIA_API ULevelLayout : public UObject
{
	GENERATED_BODY()

public:
	void Setup(const int32 rowRooms, const int32 colRooms);

	//Ensure Setup is called before Build
	void Build();

	//Ensure Build was called before this method
	ULevelData* GetLevelData();

private:
	float PERCENTAGE_OF_EXTRA_DOORS = 0.33f;

	ULevelData* levelData = nullptr;

	//Map building methods
	void ConnectAllRooms();
	void PlaceEntranceRoom();
	void PlaceBossRoom();
	void AddExtraDoors();

	FRoomLocation GetRandomLocation() const;
	FRoomLocation GetNeighborToAttachTo(const FRoomLocation& roomLocation) const;
	FRoomLocation GetRoomLocationInDirection(const FRoomLocation& randomLocation, const Direction direction) const;
};
