// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"
#include "Dysphoria.h"
#include "FPSProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Wall.h"
#include "FPSWeapon.h"
#include "DrawDebugHelpers.h"

#define LEFT -90
#define RIGHT 90

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

	//Wall jump capsule collider
	WallJumpCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WallJumpCapsuleCollider"));
	WallJumpCapsuleComponent->InitCapsuleSize(WallJumpCapsuleRadius, 80.0f); //34x88 original capsule
	WallJumpCapsuleComponent->SetCollisionProfileName(TEXT("PlayerJump"));
	WallJumpCapsuleComponent->SetupAttachment(Cast<USceneComponent>(GetCapsuleComponent()));
	
	WallJumpCapsuleComponent->CanCharacterStepUpOn = ECB_Yes;
	WallJumpCapsuleComponent->SetShouldUpdatePhysicsVolume(false);
	WallJumpCapsuleComponent->SetCanEverAffectNavigation(false);
	WallJumpCapsuleComponent->bDynamicObstacle = false;

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

	//Are we wall running?
	if (InWallRun)
	{
		DoWallRun();
	}

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

	//Wall Run
	PlayerInputComponent->BindAction("WallRun", IE_Pressed, this, &AFirstPersonController::WallRunEnable);

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
	
	InWallRun = false; //End any wall run that's happening
	WallRideLastDirection = 0.f;

	if (!this->CanJump()) { 
		UE_LOG(LogTemp, Log, TEXT("Trying to wall Jump"));

		AActor* ClosestWall = GetClosestJumpWall();

		// Wall Jump is available, if a wall was intersected and was not the last one jumped from
		if (ClosestWall && LastWallJumpedFrom != ClosestWall)
		{
			LastWallJumpedFrom = ClosestWall;
			UE_LOG(LogTemp, Log, TEXT("Launch off the wall!"));

			FVector LaunchVelocity = (GetActorForwardVector() * WallJumpHorizontalMultiplier) + FVector(0, 0, WallJumpLaunch);
			LaunchCharacter(LaunchVelocity, true, true);
		}
	}
	else {
		LastWallJumpedFrom = nullptr;
	}
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

void AFirstPersonController::DoWallRun()
{
	if (GetMovementComponent()->IsFalling())
	{
		FHitResult HitResultForward;
		FHitResult HitResultLeft;
		FHitResult HitResultRight;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);

		ECollisionChannel Channel = ECC_WorldStatic;

		FVector Start = GetActorLocation();
		FVector End = GetActorRightVector() * PlayerToWallDistance;
		FVector ForwardEnd = GetActorForwardVector() * PlayerToWallDistance;

		if (GetWorld()->LineTraceSingleByChannel(HitResultLeft, Start, Start + -End, Channel, TraceParams))
		{
			if (GetWorld()->LineTraceSingleByChannel(HitResultForward, Start, Start + ForwardEnd, Channel, TraceParams))
				AttachToWall(LEFT, WallRunSpeed, HitResultForward);
			else
				AttachToWall(LEFT, WallRunSpeed, HitResultLeft);
		}
		else if (GetWorld()->LineTraceSingleByChannel(HitResultRight, Start, Start + End, Channel, TraceParams))
		{
			if (GetWorld()->LineTraceSingleByChannel(HitResultForward, Start, Start + ForwardEnd, Channel, TraceParams))
				AttachToWall(RIGHT, WallRunSpeed, HitResultForward);
			else
				AttachToWall(RIGHT, WallRunSpeed, HitResultRight);
		}

	}

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (GetActorForwardVector() * PlayerToWallDistance),
		FColor(255, 0, 0),
		false, -1, 0,
		12.333
	);

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (GetActorRightVector() * PlayerToWallDistance),
		FColor(0, 255, 0),
		false, -1, 0,
		12.333
	);

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (-GetActorRightVector() * PlayerToWallDistance),
		FColor(0, 0, 255),
		false, -1, 0,
		12.333
	);
}

void AFirstPersonController::AttachToWall(int Direction, float WallSpeed, FHitResult HitResult)
{
	//If the wall rider turned around, then remove them from the wall
	if (WallRideLastDirection != 0.f && Direction != WallRideLastDirection)
	{
		WallRideLastDirection = 0.f;
		InWallRun = false;
		return;
	}

	//Stop Character
	GetMovementComponent()->StopMovementKeepPathing();

	// Set rotation of character 90 or -90 degrees of the normal rotation.
	FRotator RotationOf90Degrees(0, Direction, 0);
	FRotator LeftOrRightDirection = RotationOf90Degrees.RotateVector(HitResult.Normal).Rotation();
	FRotator newRotation(0, LeftOrRightDirection.Yaw, 0);

	// Next we are going to take the normal vector and grab its rotation and grab its right vector.
	// We times it by with wallspeed, then add the actors current location.
	FVector NewLoc = FRotationMatrix(HitResult.Normal.Rotation()).GetScaledAxis(EAxis::Y) * WallSpeed;
	if (Direction == LEFT) {
		NewLoc = -NewLoc + GetActorLocation();
		WallRideLastDirection = LEFT;
	}
	else {
		NewLoc = NewLoc + GetActorLocation();
		WallRideLastDirection = RIGHT;
	}
	SetActorLocation(NewLoc);
}

AActor* AFirstPersonController::GetClosestJumpWall()
{
	AActor* ClosestWall = nullptr; //Find the closest wall to the component
	TArray<AActor*> OutOverlappingActors;
	WallJumpCapsuleComponent->GetOverlappingActors(OutOverlappingActors, TSubclassOf<AWall>(AWall::StaticClass()));
	for (auto& PossibleWallActor : OutOverlappingActors)
	{
		if (!ClosestWall || FVector::Dist(PossibleWallActor->GetActorLocation(), this->GetActorLocation()) < FVector::Dist(ClosestWall->GetActorLocation(), this->GetActorLocation())) {
			ClosestWall = PossibleWallActor;
		}
	}

	return ClosestWall;
}

void AFirstPersonController::WallRunEnable()
{
	InWallRun = true;
}