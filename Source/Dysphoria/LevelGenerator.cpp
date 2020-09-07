// Copyright T Bilodeau 2020


#include "LevelGenerator.h"
#include "LevelLayout.h"
#include "LevelData.h"

void ULevelGenerator::GenerateLevel(const int32 Level)
{
	//TODO replace this level room logic with something better...
	int32 Rows = Level + 4;
	int32 Cols = Level + 4;

	ULevelLayout* Layout = NewObject<ULevelLayout>();

	Layout->Setup(Rows, Cols);

	Layout->Build();

	LevelData = Layout->GetLevelData();
}

URoomData* ULevelGenerator::GetRoom(int32 Row, int32 Col) const
{
	return LevelData->GetRoomData(Row, Col);
}

int32 ULevelGenerator::GetRows() const
{
	return LevelData->GetRows();
}

int32 ULevelGenerator::GetCols() const
{
	return LevelData->GetCols();
}

URowCol* ULevelGenerator::GetBossRoom() const
{
	URowCol* RowCol = URowCol::TurnIntoRowCol(LevelData->GetBossRoom());
	return RowCol;
}

URowCol* ULevelGenerator::GetEntrance() const
{
	URowCol* RowCol = URowCol::TurnIntoRowCol(LevelData->GetEntrance());
	return RowCol;
}