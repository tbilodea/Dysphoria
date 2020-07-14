// Copyright T Bilodeau 2020


#include "EnemyTypeUtils.h"

#include "EnemyClassType.h"
#include "EnemyData.h"

std::vector<EnemyType> EnemyTypeUtils::GetAllTypes()
{
	return { EnemyType::BODY_PARASITE, EnemyType::ADONTIS };
}

std::unique_ptr<EnemyData> EnemyTypeUtils::GetEnemyData(EnemyType type)
{
	std::unique_ptr<EnemyData> enemyData = std::make_unique<EnemyData>(EnemyData(type));

	switch (type) {
	case EnemyType::BODY_PARASITE:
		enemyData->SetData(15, 30);
		break;
	case EnemyType::ADONTIS:
		enemyData->SetData(10, 50);
		break;
	}

	return enemyData;
}

