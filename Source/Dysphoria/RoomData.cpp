// Copyright T Bilodeau 2020


#include "RoomData.h"

RoomData::RoomData()
{
}

RoomData::~RoomData()
{
}

bool RoomData::ConnectedToAnotherRoom() const
{
	return hasDoor.size() != 0;
}

bool RoomData::ConnectedToRoom(const DirectionUtils::Direction direction) const
{
	auto it = hasDoor.find(direction);
	return it != hasDoor.end();
}

void RoomData::AddDoor(const DirectionUtils::Direction direction)
{
	hasDoor.insert(direction);
}

void RoomData::RemoveDoor(const DirectionUtils::Direction direction)
{
	hasDoor.erase(direction);
}

