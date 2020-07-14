// Copyright T Bilodeau 2020


#include "PlayerEntity.h"

#include "EnemyTypeUtils.h"

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

int32 PlayerEntity::GetKillsOn(EnemyType & type)
{
	auto it = killsToEnemies.find(type);
	if (it == killsToEnemies.end()) {
		return 0;
	}
	return it->second;
}

void PlayerEntity::SetWellness(const int32 newWellness)
{
	wellness = newWellness;
}

void PlayerEntity::AddDamageTo(const EnemyType type, const int32 damage)
{
	auto it = damageToEnemies.find(type);
	if (it != damageToEnemies.end()) {
		it->second = it->second + damage;
	}
}
