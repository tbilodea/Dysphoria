#pragma once

#include "CoreMinimal.h"

/**
* Enum for each class of weapon
*/
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	REVOLVER,
	SEMI_AUTO,
	GLOCK,
	GLASS_CANNON,
	BASTARD,
	KATANA,
	KHOPESH,
	GLASS_CLAYMORE
};