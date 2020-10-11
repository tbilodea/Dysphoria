// Copyright T Bilodeau 2020


#include "PlayerEntity.h"

#include "EnemyTypeUtils.h"

APlayerEntity::APlayerEntity()
{
	//Initialize the damage and kills map to zero
	for (auto EnemyType : EnemyTypeUtils::GetAllTypes()) {
		DamageToEnemies.Add(EnemyType, 0);
		KillsToEnemies.Add(EnemyType, 0);
	}
}

int32 APlayerEntity::GetWellness() const
{
	return Wellness;
}

int32 APlayerEntity::GetKillsOn(EEnemyType & type)
{
	return *KillsToEnemies.Find(type);
}

void APlayerEntity::SetWellness(const int32 NewWellness)
{
	Wellness = NewWellness;
}

void APlayerEntity::AddDamageTo(const EEnemyType Type, const int32 Damage)
{
	auto IntPtr = DamageToEnemies.Find(Type);
	int32 NewTotalDamage = ( *IntPtr + Damage );
	DamageToEnemies.Add(Type, NewTotalDamage);
}
