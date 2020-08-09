// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <memory>

enum class EnemyType : uint8;
struct FSwarmDirective;

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
	void SetAIDirective(FSwarmDirective* directive);

	FSwarmDirective GetAIDirective() const;

	int32 GetPriority() const;
	int32 GetWellness() const;
	bool GetCanMove() const;


	//Must be overridden to be able to grab the location of the object
	virtual FVector GetLocation() = 0;
	virtual EnemyType GetEnemyType() = 0;

	virtual ~AIEntity();
protected:
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
	FSwarmDirective* CurrentDirective = nullptr;
};
