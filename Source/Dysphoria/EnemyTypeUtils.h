// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <vector>
#include <memory>

enum class EnemyType : uint8;
class EnemyData;

/**
 * Enemy type utilities
 */
class DYSPHORIA_API EnemyTypeUtils
{
public:
	static std::vector<EnemyType> GetAllTypes();

	static std::unique_ptr<EnemyData> GetEnemyData(EnemyType type);
};
