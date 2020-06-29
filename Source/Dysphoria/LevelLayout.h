// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include "DirectionUtils.h"

#include <memory>

class LevelData;
struct RoomLocation;

/**
 * Class that builds out the LevelData
 */
class DYSPHORIA_API LevelLayout
{
public:
	LevelLayout();
	~LevelLayout();

	void Setup(const int32 rowRooms, const int32 colRooms);

	//Ensure Setup is called before Build
	void Build();

	//Ensure Build was called
	std::shared_ptr<LevelData> GetLevelData();

private:
	const float PERCENTAGE_OF_EXTRA_DOORS = 0.33f;
	std::shared_ptr<LevelData> levelData = nullptr;

	//Map building methods
	void ConnectAllRooms();
	void PlaceEntranceRoom();
	void PlaceBossRoom();
	void AddExtraDoors();

	RoomLocation GetRandomLocation() const;
	RoomLocation GetNeighborToAttachTo(const RoomLocation& roomLocation) const;
	RoomLocation GetRoomLocationInDirection(const RoomLocation& randomLocation, const DirectionUtils::Direction direction) const;
};
