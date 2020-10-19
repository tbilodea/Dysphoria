// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

/**
 * Sword Weapons (Katana, Bastard, Etc)
 */
UCLASS()
class DYSPHORIA_API USword : public UWeapon
{
	GENERATED_BODY()

public:

	void SetupWeaponFor(EWeaponType NewWeaponType);

	UFUNCTION(BlueprintCallable)
	float GetTimeBetweenStrikes();

	UFUNCTION(BlueprintCallable)
	float GetTimeBetweenReset();

	UFUNCTION(BlueprintCallable)
	int32 GetDamagePerStrike();

	UFUNCTION(BlueprintCallable)
	int32 GetSwipesBeforeReset();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentSwipesLeft();

	void DecrementCurrentUses();

	bool IsSwordType();

private:
	float TimeBetweenStrikes;
	float TimeBetweenReset;
	int32 DamagePerStrike;
	int32 SwipesBeforeReset;
	int32 CurrentSwipesLeft;
};
