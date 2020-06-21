// Copyright T Bilodeau 2020


#include "RoomData.h"

RoomData::RoomData()
{
	//Init doors to false
	hasDoorMap.insert(std::make_pair(DirectionUtils::Direction::NORTH, false));
	hasDoorMap.insert(std::make_pair(DirectionUtils::Direction::EAST, false));
	hasDoorMap.insert(std::make_pair(DirectionUtils::Direction::SOUTH, false));
	hasDoorMap.insert(std::make_pair(DirectionUtils::Direction::WEST, false));
}

RoomData::~RoomData()
{
}
