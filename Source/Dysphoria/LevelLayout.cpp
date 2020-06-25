// Copyright T Bilodeau 2020

#include "LevelLayout.h"

#include "LevelData.h"
#include "RoomData.h"

LevelLayout::LevelLayout()
{
}

LevelLayout::~LevelLayout()
{
}

void LevelLayout::Setup(const int rowRooms, const int colRooms)
{
	levelData = std::make_shared<LevelData>(LevelData(rowRooms, colRooms));
}

void LevelLayout::Build()
{
	//TODO Build logic here
}

std::shared_ptr<LevelData> LevelLayout::GetLevelData()
{
	if (!levelData)
		levelData = std::make_shared<LevelData>(LevelData());

	return levelData;
}
