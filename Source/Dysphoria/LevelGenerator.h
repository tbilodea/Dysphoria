// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DYSPHORIA_API LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	//Generates the level specified
	void GenerateLevel(const int32 level);
};
