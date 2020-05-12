// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"

// Sets default values
AFirstPersonController::AFirstPersonController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();
	
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