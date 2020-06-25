// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "DirectionUtils.h"
#include <set>

/**
 * Data structure for each room in the game
 */
class DYSPHORIA_API RoomData
{
public:
	RoomData();
	~RoomData();

	// Information about this room
	bool ConnectedToAnotherRoom() const;
	bool ConnectedToRoom(const DirectionUtils::Direction direction) const;

	//Adds or removes a door to this room
	void AddDoor(const DirectionUtils::Direction direction);
	void RemoveDoor(const DirectionUtils::Direction direction);

private:
	//A set of Directions that have a door
	std::set<DirectionUtils::Direction> hasDoor = {};

};
