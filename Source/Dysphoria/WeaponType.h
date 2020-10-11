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
	KATANA,
	BASTARD,
	KHOPESH,
	GLASS_CLAYMORE
};