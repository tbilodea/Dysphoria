// Copyright T Bilodeau 2020


#include "Gun.h"
#include "WeaponType.h"

void UGun::SetupWeaponFor(EWeaponType NewWeaponType)
{
	SetWeaponType(NewWeaponType);

	switch (NewWeaponType) {
	case EWeaponType::REVOLVER:
		Recoil = 25.0f;
		TimeBetweenStrikes = 0.8f;
		ReloadTime = 4.0f;
		DamagePerStrike = 16;
		MaxAmmo = 6;
		break;
	case EWeaponType::SEMI_AUTO:
		Recoil = 8.0f;
		TimeBetweenStrikes = 0.6f;
		ReloadTime = 3.5f;
		DamagePerStrike = 9;
		MaxAmmo = 6;
		break;
	case EWeaponType::GLOCK:
		Recoil = 2.0f;
		TimeBetweenStrikes = 0.1f;
		ReloadTime = 6.0f;
		DamagePerStrike = 5;
		MaxAmmo = 15;
		break;
	case EWeaponType::GLASS_CANNON:
		Recoil = 50.0f;
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
	if (GetWeaponType() == EWeaponType::GLASS_CANNON) {
		BreakWeapon();
	}

	//Wait for the timer between firing to expire (timer cleaned up in beginDestory on Weapon)
	AllowGunToFire = false;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("OnTimerBetweenStrikesExpires"));
	//FTimerHandle& InOutHandle, FTimerDelegate const& InDelegate, float InRate, bool InbLoop
	if (!GetOuter()->GetWorld()) {
		UE_LOG(LogTemp, Warning, TEXT("instance is null"));
	} else {
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerBetweenStrikesHandle, TimerDel, TimeBetweenStrikes, false);
	}
}

void UGun::StartReload()
{
	//Wait for the timer between firing to expire (timer cleaned up in beginDestory on Weapon)
	Reloading = true;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("OnReloadTimerExpires"));
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, TimerDel, GetReloadTime(), false);
}

bool UGun::IsSwordType()
{
	return false;
}

bool UGun::TimersAllowFiring()
{
	return AllowGunToFire && !Reloading;
}

void UGun::OnTimerBetweenStrikesExpires()
{
	AllowGunToFire = true;
}

void UGun::OnReloadTimerExpires()
{
	UE_LOG(LogTemp, Warning, TEXT("Reload Finished"));
	CurrentAmmo = MaxAmmo;
	Reloading = false;
}

