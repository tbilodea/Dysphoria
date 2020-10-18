// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponType.h"
#include "Weapon.generated.h"

/**
 * Generic weapon class
 */
UCLASS(Abstract, Blueprintable)
class DYSPHORIA_API UWeapon : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetupWeaponFor(EWeaponType NewWeaponType);

	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType();

	UFUNCTION(BlueprintImplementableEvent)
	bool IsSwordType();

	//Sets the weapon to broken
	UFUNCTION(BlueprintCallable)
	void BreakWeapon();

	UFUNCTION(BlueprintCallable)
	bool IsBroken();

protected:
	UFUNCTION()
	void SetWeaponType(EWeaponType NewWeaponType);

private:
	EWeaponType WeaponType = EWeaponType::REVOLVER;
	bool IsBrokenWeapon = false;
};
