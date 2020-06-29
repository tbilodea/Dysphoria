// Copyright T Bilodeau 2020


#include "LevelGenerator.h"
#include "LevelLayout.h"

LevelGenerator::LevelGenerator()
{
}

LevelGenerator::~LevelGenerator()
{
}

void LevelGenerator::GenerateLevel(const int32 level)
{
	//TODO replace this level room logic with something better...
	int32 rows = level + 4;
	int32 cols = level + 4;

	LevelLayout layout;

	layout.Setup(rows, cols);

	layout.Build();
}
