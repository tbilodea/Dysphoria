// Copyright T Bilodeau 2020


#include "FPSWeapon.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h" 
#include "Particles/ParticleSystem.h"

// Sets default values
AFPSWeapon::AFPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
}

void AFPSWeapon::Fire()
{
	AActor* OurOwner = GetOwner();
	if (OurOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		OurOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OurOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, 1.f, ShotDirection, Hit, OurOwner->GetInstigatorController(), this, DamageType);

			// DEBUG DrawDebugSphere(GetWorld(), Hit.Location, 20, 26, FColor(181, 0, 0), true, 3, 0, 2);
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint, FVector(1.f)));
			}
		}

		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 1.f, 0, 1.f);

		if (MuzzleFlash) 
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, MuzzleSocketName);
		}
	}
}