// Copyright T Bilodeau 2020


#include "LevelData.h"
#include "RoomData.h"

struct RoomLocation {
	int row;
	int col;

	bool operator==(const RoomLocation& rhs) {
		return row == rhs.row && col == rhs.col;
	}
};

LevelData::LevelData() : ROWS(5), COLS(5)
{
}

LevelData::LevelData(int32 rows, int32 cols) : ROWS(rows), COLS(cols)
{
}

LevelData::~LevelData()
{
}

//Returns copy of levelRooms
std::map<RoomLocation, std::shared_ptr<RoomData>> LevelData::GetAllRoomDatas()
{
	return std::map<RoomLocation, std::shared_ptr<RoomData>>(levelRooms);
}

//Get the appropriate RoomData for the row col. Defaults to the first room if row/col is out of range.
std::shared_ptr<RoomData> LevelData::GetRoomData(const int row, const int col)
{
	
	return std::shared_ptr<RoomData>();
}
