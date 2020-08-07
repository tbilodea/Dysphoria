// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerEntity.h"
#include "FirstPersonController.generated.h"

class UCameraComponent;

UCLASS()
class DYSPHORIA_API AFirstPersonController : public ACharacter, public PlayerEntity
{
	GENERATED_BODY()

public:
	AFirstPersonController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	//Exposed location for the Swarm Engine
	FVector GetLocation() override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	// Gun muzzle's offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

private:
	float SpeedMultiplier = 1.0f;
	float BaseSpeed = 0.f;
	float ControllerYawSensitivity = 1.f;
	float ControllerPitchSensitivity = 1.f;

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
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
};
