// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "DirectionUtils.h"

#include "RoomData.generated.h"

/**
 * Data structure for each room in the game
 */
UCLASS(BlueprintType)
class DYSPHORIA_API URoomData : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TSet<Direction> GetDoors() const;

	// Information about this room
	bool ConnectedToAnotherRoom() const;
	bool ConnectedToRoom(const Direction direction) const;

	//Adds or removes a door to this room
	void AddDoor(const Direction direction);
	void RemoveDoor(const Direction direction);

private:
	//A set of Directions that have a door
	TSet<Direction> hasDoor;

};
