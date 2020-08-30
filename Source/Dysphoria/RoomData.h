// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "DirectionUtils.h"
#include <set>

#include "RoomData.generated.h"

/**
 * Data structure for each room in the game
 */
UCLASS()
class DYSPHORIA_API URoomData : public UObject
{
	GENERATED_BODY()

public:

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
