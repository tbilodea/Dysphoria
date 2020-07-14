#pragma once

#include "CoreMinimal.h"

/**
* Enum for each class of an enemy
* (update utilities methods if enemy is added)
*/
UENUM()
enum class EnemyType : uint8 {
	BODY_PARASITE,
	ADONTIS
};