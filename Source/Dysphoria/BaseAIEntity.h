// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIEntity.h"

#include "BaseAIEntity.generated.h"

enum class EEnemyType : uint8;

UCLASS()
class DYSPHORIA_API ABaseAIEntity : public AAIEntity
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAIEntity();

	//Swarm Engine methods
	FVector GetLocation();
	EEnemyType GetEnemyType();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser);
	
	UFUNCTION(BlueprintCallable, Category="Death")
	virtual void Die();
};
