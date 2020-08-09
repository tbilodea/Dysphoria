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
	void AddPlayers(const std::vector<PlayerEntity*>& AllPlayers);

	// Clears the old room enemies out and adds these new ones
	void AddNewRoomEnemies(const std::vector<AIEntity*>& AllRoomEnemies);

	// Add a single enemy (for ones that spawn others)
	void AddAdditionalRoomEnemy(AIEntity& EnemyToAdd);

	// Runs the decision tree and updates the AIEntity statuses
	void RunEngine();

private:
	//Weights for determining how protectors are assigned
	const float DISTANCE_WEIGHT = 1.f;
	const float ASSIGNED_PROTECTORS_WEIGHT = 30.f;

	std::vector<PlayerEntity*> Players;
	std::vector<AIEntity*> RoomEnemies;

	void FocusTree(std::vector<AIEntity*>& ToAssignDirective, std::map<EnemyType, std::vector<PlayerEntity*>>& TypeToPlayersAboveThreshold);

	void RemoveDeadEnemies();
	void RemoveUnavailablePlayers();

	PlayerEntity* FindClosestPlayer(FVector Location);
	PlayerEntity* FindClosestPlayer(FVector Location, std::vector<PlayerEntity*> PlayersToCheck);
	AIEntity* FindClosestFriend(FVector Location);

	//Map EnemyType to the Players currently hated by that type
	std::map<EnemyType, std::vector<PlayerEntity*>> BuildHatedPlayerMap();

};
