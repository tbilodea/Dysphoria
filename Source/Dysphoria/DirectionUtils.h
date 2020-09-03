// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <vector>

#include "Directions.h"

/**
 * Base Direction utility class
 */
class DYSPHORIA_API DirectionUtils
{
public:

	static Direction GetOppositeDirection(const Direction direction);

	static Direction GetNextClockwiseDirection(const Direction direction);

	static TArray<Direction> GetAllDirections();
};

