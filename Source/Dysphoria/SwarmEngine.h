// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <vector>
#include <memory>

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
	std::vector<PlayerEntity*> players;
	std::vector<AIEntity*> roomEnemies;

	void FocusTree();

	void RemoveDeadEnemies();
	void RemoveUnavailablePlayers();

	PlayerEntity* FindClosestPlayer(FVector location);
};
