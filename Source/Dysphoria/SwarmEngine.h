// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <map>
#include <vector>
#include <memory>

#include "EnemyClassType.h"

class AIEntity;
class PlayerEntity;

/**
 * The hive mind to figure out what each AIEntity needs to do to counter players
 */
class DYSPHORIA_API SwarmEngine
{
public:
	SwarmEngine();
	~SwarmEngine();

	// Adds the list of players
	void AddPlayers(const std::vector<PlayerEntity*>& allPlayers);

	// Clears the old room enemies out and adds these new ones
	void AddNewRoomEnemies(const std::vector<AIEntity*>& allRoomEnemies);

	// Add a single enemy (for ones that spawn others)
	void AddAdditionalRoomEnemy(AIEntity& enemyToAdd);

	// Runs the decision tree and updates the AIEntity statuses
	void RunEngine();

private:
	//Weights for determining how protectors are assigned
	const float DISTANCE_WEIGHT = 1.f;
	const float ASSIGNED_PROTECTORS_WEIGHT = 30.f;

	std::vector<PlayerEntity*> players;
	std::vector<AIEntity*> roomEnemies;

	void FocusTree(std::vector<AIEntity*>& toAssignDirective, std::map<EnemyType, std::vector<PlayerEntity*>>& typeToPlayersAboveThreshold);

	void RemoveDeadEnemies();
	void RemoveUnavailablePlayers();

	PlayerEntity* FindClosestPlayer(FVector location);
	PlayerEntity* FindClosestPlayer(FVector location, std::vector<PlayerEntity*> playersToCheck);
	AIEntity* FindClosestFriend(FVector location);

	//Map EnemyType to the Players currently hated by that type
	std::map<EnemyType, std::vector<PlayerEntity*>> BuildHatedPlayerMap();

};
