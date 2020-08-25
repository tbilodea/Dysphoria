// Copyright T Bilodeau 2020


#include "PlayerEntity.h"

#include "EnemyTypeUtils.h"

APlayerEntity::APlayerEntity()
{
	for (auto enemyType : EnemyTypeUtils::GetAllTypes())
	{
		int32 zero = 0;
		DamageToEnemies.insert(std::make_pair(enemyType, zero));
	}
}

int32 APlayerEntity::GetWellness() const
{
	return Wellness;
}

int32 APlayerEntity::GetKillsOn(EEnemyType & type)
{
	auto it = KillsToEnemies.find(type);
	if (it == KillsToEnemies.end()) {
		return 0;
	}
	return it->second;
}

void APlayerEntity::SetWellness(const int32 newWellness)
{
	Wellness = newWellness;
}

void APlayerEntity::AddDamageTo(const EEnemyType type, const int32 damage)
{
	auto it = DamageToEnemies.find(type);
	if (it != DamageToEnemies.end()) {
		it->second = it->second + damage;
	}
}
