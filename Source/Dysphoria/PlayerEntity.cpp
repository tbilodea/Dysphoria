// Copyright T Bilodeau 2020


#include "PlayerEntity.h"

#include "EnemyTypeUtils.h"

APlayerEntity::APlayerEntity()
{
	for (auto enemyType : EnemyTypeUtils::GetAllTypes())
	{
		int32 zero = 0;
		damageToEnemies.insert(std::make_pair(enemyType, zero));
	}
}

int32 APlayerEntity::GetWellness() const
{
	return wellness;
}

int32 APlayerEntity::GetKillsOn(EEnemyType & type)
{
	auto it = killsToEnemies.find(type);
	if (it == killsToEnemies.end()) {
		return 0;
	}
	return it->second;
}

void APlayerEntity::SetWellness(const int32 newWellness)
{
	wellness = newWellness;
}

void APlayerEntity::AddDamageTo(const EEnemyType type, const int32 damage)
{
	auto it = damageToEnemies.find(type);
	if (it != damageToEnemies.end()) {
		it->second = it->second + damage;
	}
}
