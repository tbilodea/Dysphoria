// Copyright T Bilodeau 2020


#include "BaseAIEntity.h"
#include "EnemyClassType.h"

// Sets default values
ABaseAIEntity::ABaseAIEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector ABaseAIEntity::GetLocation()
{
	return this->GetActorLocation();
}

EEnemyType ABaseAIEntity::GetEnemyType()
{
	UE_LOG(LogTemp, Warning, TEXT("Returned BODY PARASITE on abstract class BaseAIEntity.cpp, TODO clean this up"));
	return EEnemyType::BODY_PARASITE;
}

float ABaseAIEntity::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	int32 MyWellness = GetWellness();
	if (MyWellness != 0) {
		int32 AfterDamage = MyWellness - DamageAmount;

		UE_LOG(LogTemp, Warning, TEXT("AI took damage!"));

		if (AfterDamage <= 0) {
			SetWellness(0);
			DamageAmount = MyWellness;

			Die();
		}
		else {
			SetWellness(AfterDamage);
		}
	}

	return APawn::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseAIEntity::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("BaseAIEntity::Die called for %s"), *(this->GetFName().ToString()));
}