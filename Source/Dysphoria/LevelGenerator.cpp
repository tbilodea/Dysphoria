// Copyright T Bilodeau 2020


#include "LevelGenerator.h"
#include "LevelLayout.h"
#include "LevelData.h"

void ULevelGenerator::GenerateLevel(const int32 level)
{
	//TODO replace this level room logic with something better...
	int32 rows = level + 4;
	int32 cols = level + 4;

	ULevelLayout* layout = NewObject<ULevelLayout>();

	layout->Setup(rows, cols);

	layout->Build();

	levelData = layout->GetLevelData();
}

URoomData* ULevelGenerator::GetRoom(int32 Row, int32 Col) const
{
	return levelData->GetRoomData(Row, Col);
}

int32 ULevelGenerator::GetRows() const
{
	return levelData->GetRows();
}

int32 ULevelGenerator::GetCols() const
{
	return levelData->GetCols();
}
