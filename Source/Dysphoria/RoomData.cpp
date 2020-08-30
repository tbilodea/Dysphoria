// Copyright T Bilodeau 2020


#include "RoomData.h"

bool URoomData::ConnectedToAnotherRoom() const
{
	return hasDoor.size() != 0;
}

bool URoomData::ConnectedToRoom(const DirectionUtils::Direction direction) const
{
	auto it = hasDoor.find(direction);
	return it != hasDoor.end();
}

void URoomData::AddDoor(const DirectionUtils::Direction direction)
{
	hasDoor.insert(direction);
}

void URoomData::RemoveDoor(const DirectionUtils::Direction direction)
{
	hasDoor.erase(direction);
}

