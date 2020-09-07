// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelData.h"
#include "RowCol.generated.h"

/**
 * A blueprint accessible version of FRoomLocation
 */
UCLASS(Blueprintable)
class DYSPHORIA_API URowCol : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetRow(const int32 Row);
	UFUNCTION(BlueprintCallable)
	void SetCol(const int32 Col);

	UFUNCTION(BlueprintCallable)
	int32 GetRow() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCol() const;

	static URowCol* TurnIntoRowCol(FRoomLocation RoomLocation);

private:
	int32 Row = 0;
	int32 Col = 0;
};
