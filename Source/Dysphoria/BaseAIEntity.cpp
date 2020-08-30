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

// Called when the game starts or when spawned
void ABaseAIEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAIEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

