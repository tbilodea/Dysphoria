// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"
#include "Dysphoria.h"
#include "FPSProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "FPSWeapon.h"

// Sets default values
AFirstPersonController::AFirstPersonController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera setup
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCameraComponent->SetupAttachment(Cast<USceneComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(-50.f, 0.f, 50.f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	// FPS Mesh
	//FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	 //Only the owning player sees this mesh. TODO change when setting up multiplayer?
	//FPSMesh->SetOnlyOwnerSee(true);
	//FPSMesh->SetupAttachment(FPSCameraComponent);
	//// Disable some environmental shadowing to preserve the illusion of having a single mesh.
	//FPSMesh->bCastDynamicShadow = false;
	//FPSMesh->CastShadow = false;

	//Initialize Weapons
	GunWeapon = CreateDefaultSubobject<UGun>(TEXT("Gun"));
	SwordWeapon = CreateDefaultSubobject<USword>(TEXT("Sword"));

	GunWeapon->SetupWeaponFor(EWeaponType::REVOLVER);
	SwordWeapon->SetupWeaponFor(EWeaponType::BASTARD);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFirstPersonController::SwapWeapon()
{
	UE_LOG(LogFPSController, Log, TEXT("Swapping weapons before swap UsingGun is %s"), UsingGun ? TEXT("true") : TEXT("false"));
	UsingGun = !UsingGun;
}

UWeapon* AFirstPersonController::GetCurrentWeapon()
{
	if (UsingGun)
		return GunWeapon;
	else
		return SwordWeapon;
}

// Called every frame
void AFirstPersonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Setup movement bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonController::MoveRight);

	//Rotation bindings mouse
	PlayerInputComponent->BindAxis("LookUp", this, &AFirstPersonController::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFirstPersonController::AddControllerYawInput);

	//Rotation bindings controller
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPersonController::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonController::AddControllerYawInput);

	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonController::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonController::StopJump);

	//Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFirstPersonController::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFirstPersonController::EndCrouch);

	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFirstPersonController::Fire);

	//Brings up level map
	PlayerInputComponent->BindAction("ToggleMap", IE_Pressed, this, &AFirstPersonController::ToggleMap).bExecuteWhenPaused = true;

	//Switch Weapon
	PlayerInputComponent->BindAction("SwapToGun", IE_Pressed, this, &AFirstPersonController::SwapToGun);
	PlayerInputComponent->BindAction("SwapToSword", IE_Pressed, this, &AFirstPersonController::SwapToSword);
	PlayerInputComponent->BindAction("SwapWeapon", IE_Pressed, this, &AFirstPersonController::SwapWeapon);

	//Reload
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFirstPersonController::Reload);
}

void AFirstPersonController::BeginCrouch()
{
	Crouch();
}

void AFirstPersonController::EndCrouch()
{
	UnCrouch();
}

void AFirstPersonController::MoveForward(float Value)
{
	float clampForward = SpeedMultiplier * Value + BaseSpeed;
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFirstPersonController::MoveRight(float Value)
{
	float clampForward = SpeedMultiplier * Value + BaseSpeed;
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFirstPersonController::TurnRate(float Value)
{
	AddControllerYawInput(Value * ControllerYawSensitivity);
}

void AFirstPersonController::LookUpRate(float Value)
{
	AddControllerPitchInput(Value * ControllerPitchSensitivity);
}

void AFirstPersonController::StartJump()
{
	bPressedJump = true;
}

void AFirstPersonController::StopJump()
{
	bPressedJump = false;
}

void AFirstPersonController::Fire()
{
	UE_LOG(LogFPSController, Warning, TEXT("Fire Pressed"));

	// Determine if sword or gun is out
	if (UsingGun) {
		//Gun has to have ammo to utilize and not be flagged as broken
		if (GunWeapon->IsBroken() || GunWeapon->GetCurrentAmmo() <= 0) {
			// Out of ammo

			//TODO audio empty click of whatever type?

			return;
		}

		if (GunWeapon->TimersAllowFiring()) {
			GunWeapon->DecrementCurrentUses();

			// Get the camera transform.
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			// Transform MuzzleOffset from camera space to world space.
			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
			FRotator MuzzleRotation = CameraRotation;

			// Create Recoil
			MuzzleRotation.Pitch += GunWeapon->GetRecoil();

			//TODO switch Projectile setup to be just raytrace
			FireFPSWeapon();
		}
	}
	else {

		//TODO figure out sword hitboxes (one in front, one below)

	}

	// Attempt to fire a projectile.
	/*if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// Skew the aim to be slightly upwards.
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			// Spawn the projectile at the muzzle.
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}*/
}

FVector AFirstPersonController::GetLocation()
{
	return this->GetActorLocation();
}

FVector AFirstPersonController::GetPawnViewLocation() const
{
	if (FPSCameraComponent)
	{
		return FPSCameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void AFirstPersonController::SwapToGun()
{
	if (!UsingGun) {
		SwapWeapon();
	}
}

void AFirstPersonController::SwapToSword()
{
	if (UsingGun) {
		SwapWeapon();
	}
}

void AFirstPersonController::Reload()
{
	if (UsingGun) {
		GunWeapon->StartReload();
	}
}
