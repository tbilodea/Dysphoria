// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "EnemyTypeUtils.h"
#include <map>

/**
 * The Player Character/Controller class should inherit this so the SwarmEngine system can
 * track what decisions it should make
 */
class DYSPHORIA_API PlayerEntity
{
public:
	PlayerEntity();
	~PlayerEntity();

	int32 GetWellness() const;

protected:
	virtual FVector GetLocationCoordinates() = 0;

	void SetWellness(const int32 newWellness);
	void AddDamageTo(const EnemyTypeUtils::EnemyType type, const int32 damage);

private:
	//Damage done to enemy types
	std::map<EnemyTypeUtils::EnemyType, int32> damageToEnemies = std::map<EnemyTypeUtils::EnemyType, int32>();

	//wellness of player
	int32 wellness;
	
};
