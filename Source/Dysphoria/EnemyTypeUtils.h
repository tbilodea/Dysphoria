// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include <vector>

/**
 * Enemy type utilities
 */
class DYSPHORIA_API EnemyTypeUtils
{
public:

	/**
	* Enum for each class of an enemy
	* (update utilities methods if enemy is added)
	*/
	UENUM()
		enum class EnemyType : uint8 {
		BODY_PARASITE,
		ADONTIS
	};

	static std::vector<EnemyType> GetAllTypes();
};
