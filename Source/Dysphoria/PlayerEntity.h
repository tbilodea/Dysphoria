// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <map>
#include "EnemyClassType.h"

#include "PlayerEntity.generated.h"

/**
 * The Player Character/Controller class should inherit this so the SwarmEngine system can
 * track what decisions it should make
 */
UCLASS(Abstract, BlueprintType)
class DYSPHORIA_API APlayerEntity : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerEntity();

	int32 GetWellness() const;
	int32 GetKillsOn(EEnemyType& type);

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetLocation();

protected:

	void SetWellness(const int32 newWellness);
	void AddDamageTo(const EEnemyType type, const int32 damage);

private:
	//Damage done to enemy types
	std::map<EEnemyType, int32> damageToEnemies = std::map<EEnemyType, int32>();
	std::map<EEnemyType, int32> killsToEnemies = std::map<EEnemyType, int32>();

	//wellness of player
	int32 wellness = 100;
	
};
