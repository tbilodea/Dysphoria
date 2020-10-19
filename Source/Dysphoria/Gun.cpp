// Copyright T Bilodeau 2020


#include "Gun.h"
#include "WeaponType.h"

void UGun::SetupWeaponFor(EWeaponType NewWeaponType)
{
	SetWeaponType(NewWeaponType);

	switch (NewWeaponType) {
	case EWeaponType::REVOLVER:
		Recoil = 0.3f;
		TimeBetweenStrikes = 0.8f;
		ReloadTime = 4.0f;
		DamagePerStrike = 16;
		MaxAmmo = 6;
		break;
	case EWeaponType::SEMI_AUTO:
		Recoil = 0.2f;
		TimeBetweenStrikes = 0.6f;
		ReloadTime = 3.5f;
		DamagePerStrike = 9;
		MaxAmmo = 6;
		break;
	case EWeaponType::GLOCK:
		Recoil = 0.08f;
		TimeBetweenStrikes = 0.1f;
		ReloadTime = 6.0f;
		DamagePerStrike = 5;
		MaxAmmo = 15;
		break;
	case EWeaponType::GLASS_CANNON:
		Recoil = 1.0f;
		TimeBetweenStrikes = 5.0f;
		ReloadTime = 10.0f;
		DamagePerStrike = 200;
		MaxAmmo = 1;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Encountered Sword or other EWeaponType type in UGun::SetupWeaponFor [%i]"), NewWeaponType);
	}

	CurrentAmmo = MaxAmmo;

}

float UGun::GetRecoil()
{
	return Recoil;
}

float UGun::GetTimeBetweenStrikes()
{
	return TimeBetweenStrikes;
}

float UGun::GetReloadTime()
{
	return ReloadTime;
}

int32 UGun::GetDamagePerStrike()
{
	return DamagePerStrike;
}

int32 UGun::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int32 UGun::GetMaxAmmo()
{
	return MaxAmmo;
}

void UGun::DecrementCurrentUses()
{
	CurrentAmmo--;
}

bool UGun::IsSwordType()
{
	return false;
}