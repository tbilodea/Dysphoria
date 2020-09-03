// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include "LevelGenerator.generated.h"

class ULevelData;

/**
 * Generates the level based upon what level needs to be generated
 */
UCLASS(Blueprintable)
class DYSPHORIA_API ULevelGenerator : public UObject
{
	GENERATED_BODY()

public:

	//Generates the level specified
	UFUNCTION(BlueprintCallable)
	void GenerateLevel(const int32 level);

	UFUNCTION(BlueprintCallable)
	URoomData* GetRoom(int32 row, int32 col) const;

	UFUNCTION(BlueprintCallable)
	int32 GetRows() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCols() const;


private:
	ULevelData* levelData = nullptr;
};
