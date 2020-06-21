// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "DirectionUtils.h"
#include <map>

/**
 * Data structure for each room in the game
 */
class DYSPHORIA_API RoomData
{
public:
	RoomData();
	~RoomData();

private:
	//Direction int to boolean defining if that direction has a door
	std::map<DirectionUtils::Direction, bool> hasDoorMap;
};
