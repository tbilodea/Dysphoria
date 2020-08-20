// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyClasstype.h"
#include <memory>

#include "AIEntity.generated.h"

class USwarmDirective;

/**
 * Abstract class for the data associated with an entity that needs to take generic AI decisions
 * 
 * Inherit this class into the object you want to have the AI take into consideration
 *
 * Defaults to priority=1, wellness=100, canMove=true
 */
UCLASS(Abstract, BlueprintType)
class DYSPHORIA_API AAIEntity : public ACharacter
{
	GENERATED_BODY()

public:
	void SetAIDirective(USwarmDirective* directive);

	UFUNCTION(BlueprintCallable)
	USwarmDirective* GetAIDirective();

	int32 GetPriority() const;
	int32 GetWellness() const;
	bool GetCanMove() const;

	//Must be overridden to be able to grab the location of the object
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetLocation();

	UFUNCTION(BlueprintImplementableEvent)
	EEnemyType GetEnemyType();

protected:
	UFUNCTION(BlueprintCallable)
	void Initialize();

	void SetPriority(const int32 Priority);
	void SetWellness(const int32 Wellness);
	void SetCanMove(const bool CanMove);

private:
	
	// Priority level to keep alive (AI enemy) or kill (player)
	int32 Priority = 1;

	// Current entity wellness (0-100)
	int32 Wellness = 100;

	// If this entity can move around
	bool CanMove = true;

	// The directive state of the entity currently
	USwarmDirective* CurrentDirective = nullptr;
};
