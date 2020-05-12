// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonController.generated.h"

UCLASS()
class DYSPHORIA_API AFirstPersonController : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:

	virtual void BeginPlay() override;

private:
	float SpeedMultiplier = 1.0f;
	float BaseSpeed = 0.f;
	float ControllerYawSensitivity = 1.f;
	float ControllerPitchSensitivity = 1.f;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void TurnRate(float Value);

	UFUNCTION()
	void LookUpRate(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();
};
