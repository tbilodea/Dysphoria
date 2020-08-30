// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include "LevelGenerator.generated.h"

/**
 * 
 */
UCLASS()
class DYSPHORIA_API ULevelGenerator : public UObject
{
	GENERATED_BODY()

public:

	//Generates the level specified
	UFUNCTION(BlueprintCallable)
	void GenerateLevel(const int32 level);
};
