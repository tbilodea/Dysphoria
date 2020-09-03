// Copyright T Bilodeau 2020


#include "RoomData.h"

TSet<Direction> URoomData::GetDoors() const
{
	return hasDoor;
}

bool URoomData::ConnectedToAnotherRoom() const
{
	return hasDoor.Num() != 0;
}

bool URoomData::ConnectedToRoom(const Direction direction) const
{
	auto it = hasDoor.Find(direction);
	return it != NULL;
}

void URoomData::AddDoor(const Direction direction)
{
	hasDoor.Add(direction);
}

void URoomData::RemoveDoor(const Direction direction)
{
	hasDoor.Remove(direction);
}

