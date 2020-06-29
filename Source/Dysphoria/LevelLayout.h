// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
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

	void Setup(const int rowRooms, const int colRooms);

	//Ensure Setup is called before Build
	void Build();

	//Ensure Build was called
	std::shared_ptr<LevelData> GetLevelData();

private:
	std::shared_ptr<LevelData> levelData = nullptr;

	//Map building methods
	void ConnectAllRooms();
	void PlaceEntranceRoom();
	void PlaceBossRoom();
	void AddExtraDoors();

	RoomLocation GetStartRoomLocation() const;
	RoomLocation GetNeighborToAttachTo(const RoomLocation& roomLocation);

};
