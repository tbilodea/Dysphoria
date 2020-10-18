// Copyright T Bilodeau 2020


#include "Weapon.h"

void UWeapon::SetupWeaponFor(EWeaponType NewWeaponType) 
{
	//Don't use
}

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

void UWeapon::SetWeaponType(EWeaponType NewWeaponType)
{
	WeaponType = NewWeaponType;
}
