// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include <memory>
#include <map>

class RoomData;

/*
* Struct for defining each rooms location, since this is data that doesn't need to be in the RoomData
*/
struct RoomLocation {
	int row;
	int col;

	bool operator==(const RoomLocation& rhs) const {
		return row == rhs.row && col == rhs.col;
	}

	bool operator>(const RoomLocation& rhs) const {
		if (row != rhs.row) {
			if (row > rhs.row) {
				return true;
			}
			else if (row < rhs.row) {
				return false;
			}
		}
		//rows are equal, so use columns
		return col > rhs.col;
	}

	bool operator<(const RoomLocation& rhs) const {
		if (row != rhs.row) {
			if (row < rhs.row) {
				return true;
			}
			else if (row > rhs.row) {
				return false;
			}
		}
		//rows are equal, so use columns
		return col < rhs.col;
	}
};

/**
 * LevelData exists to store the data about a level
 *
 * Row/Col definitions of the RoomLocation are based in Quadrant I ([0,0] being the bottomleft or SOUTH-WEST corner)
 * 
 */
class DYSPHORIA_API LevelData
{
public:
	LevelData();
	LevelData(int32 rows, int32 cols);
	~LevelData();

	//Returns copy of levelRooms
	std::map<RoomLocation, std::shared_ptr<RoomData>> GetAllRoomDatas();

	//Returns a pointer to the RoomData at row/col
	std::shared_ptr<RoomData> GetRoomData(const int row, const int col);

	void InitializeLevelDataMap();

private:
	const int32 ROWS;
	const int32 COLS;

	std::map<RoomLocation, std::shared_ptr<RoomData>> levelRooms;

};
