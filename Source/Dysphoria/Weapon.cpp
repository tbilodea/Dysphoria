// Copyright T Bilodeau 2020


#include "Weapon.h"

#include "Engine/World.h"

EWeaponType UWeapon::GetWeaponType()
{
	return WeaponType;
}

void UWeapon::BreakWeapon()
{
	if (WeaponType == EWeaponType::GLASS_CANNON || WeaponType == EWeaponType::GLASS_CLAYMORE)
	{
		IsBrokenWeapon = true;
	}
}

bool UWeapon::IsBroken()
{
	return IsBrokenWeapon;
}

void UWeapon::BeginDestroy()
{
	Super::BeginDestroy();

	//Clean up timers
	if (GetWorld() && this) {
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void UWeapon::SetWeaponType(EWeaponType NewWeaponType)
{
	WeaponType = NewWeaponType;
}
