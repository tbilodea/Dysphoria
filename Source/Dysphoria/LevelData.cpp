// Copyright T Bilodeau 2020


#include "LevelData.h"
#include "RoomData.h"

LevelData::LevelData() : ROWS(5), COLS(5)
{
}

LevelData::LevelData(int32 rows, int32 cols) : ROWS(rows), COLS(cols)
{
}

LevelData::~LevelData()
{
}

//Returns copy of levelRooms map
std::map<RoomLocation, std::shared_ptr<RoomData>> LevelData::GetAllRoomDatas()
{
	return std::map<RoomLocation, std::shared_ptr<RoomData>>(levelRooms);
}

//Get the appropriate RoomData for the row col. Defaults to the first room if row/col is out of range.
std::shared_ptr<RoomData> LevelData::GetRoomData(const int row, const int col)
{
	RoomLocation location;
	location.row = row;
	location.col = col;

	auto it = levelRooms.find(location);
	if (it != levelRooms.end()) {
		return (it->second);
	}
	else 
	{
		location.row = 0;
		location.col = 0;

		auto it2 = levelRooms.find(location);

		return (it2->second);
	}
}
