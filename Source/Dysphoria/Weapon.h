// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponType.h"
#include "TimerManager.h" 
#include "Weapon.generated.h"

/**
 * Generic weapon class
 */
UCLASS(Abstract, Blueprintable)
class DYSPHORIA_API UWeapon : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupWeaponFor(EWeaponType NewWeaponType);

	UFUNCTION(BlueprintImplementableEvent)
	void DecrementCurrentUses();

	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsSwordType();

	//Sets the weapon to broken
	UFUNCTION(BlueprintCallable)
	void BreakWeapon();

	UFUNCTION(BlueprintCallable)
	bool IsBroken();

	void BeginDestroy();

protected:
	UFUNCTION()
	void SetWeaponType(EWeaponType NewWeaponType);

private:
	EWeaponType WeaponType = EWeaponType::REVOLVER;
	bool IsBrokenWeapon = false;
};
