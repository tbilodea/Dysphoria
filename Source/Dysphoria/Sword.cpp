// Copyright T Bilodeau 2020


#include "Sword.h"
#include "WeaponType.h"

void USword::SetupWeaponFor(EWeaponType NewWeaponType)
{
	SetWeaponType(NewWeaponType);

	switch (NewWeaponType) {
	case EWeaponType::KATANA:
		TimeBetweenStrikes = 0.3f;
		TimeBetweenReset = 0.0f;
		DamagePerStrike = 11;
		SwipesBeforeReset = 10;
		break;
	case EWeaponType::BASTARD:
		TimeBetweenStrikes = 0.5f;
		TimeBetweenReset = 1.0f;
		DamagePerStrike = 21;
		SwipesBeforeReset = 3;
		break;
	case EWeaponType::KHOPESH:
		TimeBetweenStrikes = 0.9f;
		TimeBetweenReset = 4.0f;
		DamagePerStrike = 12;
		SwipesBeforeReset = 4;
		break;
	case EWeaponType::GLASS_CLAYMORE:
		TimeBetweenStrikes = 2.0f;
		TimeBetweenReset = 1.0f;
		DamagePerStrike = 200;
		SwipesBeforeReset = 2;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Encountered Gun or other EWeaponType type in USword::SetupWeaponFor [%i]"), NewWeaponType);
	}
}

float USword::GetTimeBetweenStrikes()
{
	return TimeBetweenStrikes;
}

float USword::GetTimeBetweenReset()
{
	return TimeBetweenReset;
}

int32 USword::GetDamagePerStrike()
{
	return DamagePerStrike;
}

int32 USword::GetSwipesBeforeReset()
{
	return SwipesBeforeReset;
}

bool USword::IsSwordType()
{
	return true;
}