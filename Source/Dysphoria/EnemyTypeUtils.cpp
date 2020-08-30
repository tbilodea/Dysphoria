// Copyright T Bilodeau 2020


#include "EnemyTypeUtils.h"

#include "EnemyClassType.h"
#include "EnemyData.h"

std::vector<EEnemyType> EnemyTypeUtils::GetAllTypes()
{
	return { EEnemyType::BODY_PARASITE, EEnemyType::ADONTIS };
}

std::unique_ptr<EnemyData> EnemyTypeUtils::GetEnemyData(EEnemyType type)
{
	std::unique_ptr<EnemyData> enemyData = std::make_unique<EnemyData>(EnemyData(type));

	switch (type) {
	case EEnemyType::BODY_PARASITE:
		enemyData->SetData(15, 30);
		break;
	case EEnemyType::ADONTIS:
		enemyData->SetData(10, 50);
		break;
	}

	return enemyData;
}

