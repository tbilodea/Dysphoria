// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <map>

#include "EnemyClassType.h"

/**
 * The Player Character/Controller class should inherit this so the SwarmEngine system can
 * track what decisions it should make
 */
class DYSPHORIA_API PlayerEntity
{
public:
	PlayerEntity();
	virtual ~PlayerEntity() = 0;

	int32 GetWellness() const;
	int32 GetKillsOn(EnemyType& type);

	virtual FVector GetLocation() = 0;

protected:

	void SetWellness(const int32 newWellness);
	void AddDamageTo(const EnemyType type, const int32 damage);

private:
	//Damage done to enemy types
	std::map<EnemyType, int32> damageToEnemies = std::map<EnemyType, int32>();
	std::map<EnemyType, int32> killsToEnemies = std::map<EnemyType, int32>();

	//wellness of player
	int32 wellness;
	
};
