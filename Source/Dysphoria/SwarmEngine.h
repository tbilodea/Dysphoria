// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <map>
#include <vector>
#include <memory>

#include "EnemyClassType.h"

class AAIEntity;
class APlayerEntity;

/**
 * The hive mind to figure out what each AIEntity needs to do to counter players
 */
class DYSPHORIA_API SwarmEngine
{
public:
	SwarmEngine();
	~SwarmEngine();

	// Adds the list of players
	void AddPlayers(const std::vector<APlayerEntity*>& AllPlayers);

	// Clears the old room enemies out and adds these new ones
	void AddNewRoomEnemies(const std::vector<AAIEntity*>& AllRoomEnemies);

	// Add a single enemy (for ones that spawn others)
	void AddAdditionalRoomEnemy(AAIEntity& EnemyToAdd);

	// Runs the decision tree and updates the AIEntity statuses
	void RunEngine();

private:
	//Weights for determining how protectors are assigned
	const float DISTANCE_WEIGHT = 1.f;
	const float ASSIGNED_PROTECTORS_WEIGHT = 30.f;

	std::vector<APlayerEntity*> Players;
	std::vector<AAIEntity*> RoomEnemies;

	void FocusTree(std::vector<AAIEntity*>& ToAssignDirective, std::map<EEnemyType, std::vector<APlayerEntity*>>& TypeToPlayersAboveThreshold);

	void RemoveDeadEnemies();
	void RemoveUnavailablePlayers();

	APlayerEntity* FindClosestPlayer(FVector Location);
	APlayerEntity* FindClosestPlayer(FVector Location, std::vector<APlayerEntity*> PlayersToCheck);
	AAIEntity* FindClosestFriend(FVector Location);

	//Map EnemyType to the Players currently hated by that type
	std::map<EEnemyType, std::vector<APlayerEntity*>> BuildHatedPlayerMap();

};
