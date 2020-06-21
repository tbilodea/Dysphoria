// Copyright T Bilodeau 2020


#include "DirectionUtils.h"

DirectionUtils::Direction DirectionUtils::GetOppositeDirection(const DirectionUtils::Direction direction)
{
	switch (direction) {
		case Direction::NORTH:
				return Direction::SOUTH;
		case Direction::EAST:
				return Direction::WEST;
		case Direction::SOUTH:
				return Direction::NORTH;
		case Direction::WEST:
				return Direction::EAST;
	}

	return Direction::NORTH;
}

DirectionUtils::Direction DirectionUtils::GetNextClockwiseDirection(const DirectionUtils::Direction direction)
{
	switch (direction) {
	case Direction::NORTH:
		return Direction::WEST;
	case Direction::EAST:
		return Direction::SOUTH;
	case Direction::SOUTH:
		return Direction::WEST;
	case Direction::WEST:
		return Direction::NORTH;
	}

	return Direction::NORTH;
}
