// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Gun.generated.h"

/**
 * Hand Gun Weapons (REVOLVER, GLOCK, ETC)
 */
UCLASS()
class DYSPHORIA_API UGun : public UWeapon
{
	GENERATED_BODY()

public:

	void SetupWeaponFor(EWeaponType NewWeaponType);

	UFUNCTION(BlueprintCallable)
	float GetRecoil();

	UFUNCTION(BlueprintCallable)
	float GetTimeBetweenStrikes();

	UFUNCTION(BlueprintCallable)
	float GetReloadTime();

	UFUNCTION(BlueprintCallable)
	int32 GetDamagePerStrike();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmo();

	UFUNCTION(BlueprintCallable)
	int32 GetMaxAmmo();

	bool IsSwordType();

private:
	float Recoil;
	float TimeBetweenStrikes;
	float ReloadTime;
	int32 DamagePerStrike;
	int32 MaxAmmo;
	int32 CurrentAmmo;
};
