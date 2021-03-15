// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerEntity.h"
#include "WeaponType.h"
#include "Gun.h"
#include "Sword.h"
#include "FirstPersonController.generated.h"

class UCameraComponent;
class AFPSWeapon;

UCLASS()
class DYSPHORIA_API AFirstPersonController : public APlayerEntity
{
	GENERATED_BODY()

public:
	AFirstPersonController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BeginCrouch();

	void EndCrouch();

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	//Exposed location for the Swarm Engine
	FVector GetLocation();

	virtual FVector GetPawnViewLocation() const override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	// Gun muzzle's offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void FireFPSWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Overlay")
	void ToggleMap();

	UFUNCTION(BlueprintCallable)
	void SwapWeapon();

	UFUNCTION(BlueprintCallable)
	UWeapon* GetCurrentWeapon();

private:
	float SpeedMultiplier = 1.0f;
	float BaseSpeed = 0.f;
	float ControllerYawSensitivity = 1.f;
	float ControllerPitchSensitivity = 1.f;
	bool InWallRun = false;
	int32 WallRideLastDirection = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WallJumpLaunch = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WallJumpHorizontalMultiplier = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WallJumpCapsuleRadius = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float PlayerToWallDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WallRunSpeed = 300.f;

	AActor* LastWallJumpedFrom;

	// Default types of Gun/Sword
	UGun* GunWeapon;
	USword* SwordWeapon;

	// Current Weapon in use
	bool UsingGun = true;

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// For wall jump logic
	UCapsuleComponent* WallJumpCapsuleComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FPSMesh;

	UFUNCTION()
	void TurnRate(float Value);

	UFUNCTION()
	void LookUpRate(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void Fire();  //For firing projectiles

	UFUNCTION()
	void SwapToGun();

	UFUNCTION()
	void SwapToSword();

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void DoWallRun();

	UFUNCTION()
	void AttachToWall(int Direction, float WallSpeed, FHitResult HitResult);

	AActor* GetClosestJumpWall();

	UFUNCTION()
	void WallRunEnable();
};
