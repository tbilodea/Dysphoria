// Copyright T Bilodeau 2020


#include "LevelGenerator.h"
#include "LevelLayout.h"

void ULevelGenerator::GenerateLevel(const int32 level)
{
	//TODO replace this level room logic with something better...
	int32 rows = level + 4;
	int32 cols = level + 4;

	ULevelLayout layout;

	layout.Setup(rows, cols);

	layout.Build();
}
