// Copyright T Bilodeau 2020


#include "RoomData.h"

TSet<Direction> URoomData::GetDoors() const
{
	return HasDoor;
}

bool URoomData::ConnectedToAnotherRoom() const
{
	return HasDoor.Num() != 0;
}

bool URoomData::ConnectedToRoom(const Direction Direction) const
{
	auto It = HasDoor.Find(Direction);
	return It != NULL;
}

void URoomData::AddDoor(const Direction Direction)
{
	HasDoor.Add(Direction);
}

void URoomData::RemoveDoor(const Direction Direction)
{
	HasDoor.Remove(Direction);
}

