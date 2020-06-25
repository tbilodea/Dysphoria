// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "DirectionUtils.h"
#include <set>
#include <memory>

/**
 * Data structure for each room in the game
 */
class DYSPHORIA_API RoomData
{
public:
	RoomData();
	~RoomData();

	bool ConnectedToAnotherRoom() const;

	bool ConnectedToRoom(const DirectionUtils::Direction direction) const;

	void AddDoor(const DirectionUtils::Direction direction);

	void RemoveDoor(const DirectionUtils::Direction direction);

private:
	//A set of Directions that have a door
	std::set<DirectionUtils::Direction> hasDoor = {};

};
