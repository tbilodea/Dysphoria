// Copyright T Bilodeau 2020


#include "EnemyData.h"
#include "EnemyClassType.h"

EnemyData::EnemyData(EEnemyType newType)
{
	type = newType;
}

EnemyData::~EnemyData()
{
}

EEnemyType EnemyData::GetType() const
{
	return type;
}

int32 EnemyData::GetHateLimit() const
{
	return hateLimit;
}

void EnemyData::SetData(int32 newHateLimit, int32 newHealth)
{
	hateLimit = newHateLimit;
	health = newHealth;
}
