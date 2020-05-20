// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"
#include "FPSProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFirstPersonController::AFirstPersonController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera setup
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCameraComponent->SetupAttachment(Cast<USceneComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;

	// FPS Mesh
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// Only the owning player sees this mesh. TODO change when setting up multiplayer?
	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->SetupAttachment(FPSCameraComponent);
	// Disable some environmental shadowing to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
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

	//Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonController::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonController::StopJump);

	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFirstPersonController::Fire);
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
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire w Proj Class"));
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
			UE_LOG(LogTemp, Warning, TEXT("Fire w/proj class and world"));
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			// Spawn the projectile at the muzzle.
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				UE_LOG(LogTemp, Warning, TEXT("created projectile"));
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
