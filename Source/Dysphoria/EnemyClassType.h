#pragma once

#include "CoreMinimal.h"

/**
* Enum for each class of an enemy
* (update utilities methods if enemy is added)
*/
UENUM(BlueprintType)
enum class EEnemyType : uint8 {
	BODY_PARASITE,
	ADONTIS
};