// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include <vector>

/**
 * Base Direction class
 */
class DYSPHORIA_API DirectionUtils
{
public:

	UENUM()
	enum class Direction : int8 {
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3
	};

	static Direction GetOppositeDirection(const Direction direction);

	static Direction GetNextClockwiseDirection(const Direction direction);

	static std::vector<Direction> GetAllDirections();
};

