// Copyright T Bilodeau 2020


#include "PlayerEntity.h"

PlayerEntity::PlayerEntity()
{
	for (auto enemyType : EnemyTypeUtils::GetAllTypes())
	{
		int32 zero = 0;
		damageToEnemies.insert(std::make_pair(enemyType, zero));
	}
}

PlayerEntity::~PlayerEntity()
{
}

int32 PlayerEntity::GetWellness() const
{
	return wellness;
}

void PlayerEntity::SetWellness(const int32 newWellness)
{
	wellness = newWellness;
}

void PlayerEntity::AddDamageTo(const EnemyTypeUtils::EnemyType type, const int32 damage)
{
	auto it = damageToEnemies.find(type);
	if (it != damageToEnemies.end()) {
		it->second = it->second + damage;
	}
}
