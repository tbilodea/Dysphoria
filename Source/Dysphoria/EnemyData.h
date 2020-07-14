// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include <vector>

enum class EnemyType : uint8;

/**
 * Basic data about an enemy type
 */
class DYSPHORIA_API EnemyData
{
public:
	EnemyData(EnemyType type);
	~EnemyData();

	EnemyType GetType() const;
	int32 GetHateLimit() const;
	
	//int32 of enemies killed before "hate", int32 of base health
	void SetData(int32 newHateLimit, int32 newHealth);

private:
	//The type of enemy this class represents
	EnemyType type;

	//Limit of killed enemies of this type before "hate" activates in the SwarmEngine logic
	int32 hateLimit = 0;

	//Standard amount of health the enemy has
	int32 health = 50;


};
