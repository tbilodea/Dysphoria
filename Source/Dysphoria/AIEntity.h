// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <memory>

enum class EnemyType : uint8;
class AIDirective;

/**
 * Abstract class for the data associated with an entity that needs to take generic AI decisions
 * 
 * Inherit this class into the object you want to have the AI take into consideration
 *
 * Defaults to priority=1, wellness=100, canMove=true
 */
class DYSPHORIA_API AIEntity
{

public:
	void SetAIDirective(AIDirective directive);

	AIDirective GetAIDirective() const;

	int32 GetPriority() const;
	int32 GetWellness() const;
	bool CanMove() const;


	//Must be overridden to be able to grab the location of the object
	virtual FVector GetLocation() = 0;
	virtual EnemyType GetEnemyType() = 0;

protected:
	void Initialize();

	void SetPriority(const int32 priority);
	void SetWellness(const int32 wellness);
	void SetCanMove(const bool canMove);

private:
	
	// Priority level to keep alive (AI enemy) or kill (player)
	int32 priority = 1;

	// Current entity wellness (0-100)
	int32 wellness = 100;

	// If this entity can move around
	bool canMove = true;

	// The directive state of the entity currently
	AIDirective* currentDirective = nullptr;
};
