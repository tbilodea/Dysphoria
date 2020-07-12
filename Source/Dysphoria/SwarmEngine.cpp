// Copyright T Bilodeau 2020


#include "SwarmEngine.h"
#include "AIEntity.h"
#include "PlayerEntity.h"
#include "AIDirective.h"

SwarmEngine::SwarmEngine()
{
}

SwarmEngine::~SwarmEngine()
{
}

void SwarmEngine::AddPlayers(const std::vector<PlayerEntity*>& allPlayers)
{
	players.clear();

	players.insert(allPlayers.begin(), allPlayers.end(), allPlayers.end());
}

void SwarmEngine::AddNewRoomEnemies(const std::vector<AIEntity*>& newRoomEnemies)
{
	roomEnemies.clear();

	roomEnemies.insert(newRoomEnemies.begin(), newRoomEnemies.end(), newRoomEnemies.end());
}

void SwarmEngine::AddAdditionalRoomEnemy(AIEntity& enemyToAdd)
{
	roomEnemies.emplace_back(&enemyToAdd);
}

//Updates the AIDirectives for each entity
void SwarmEngine::RunEngine()
{
	RemoveDeadEnemies();
	RemoveUnavailablePlayers();

	// Check number of AI
	if (roomEnemies.size() == 1) {
		FocusTree();
	}
	else {
		std::vector<std::shared_ptr<AIEntity>> protectees = {};
		//Determine which have reached "protection level" which is priority 1 or priority 2 if the wellness hits a mark
		for (auto& roomEnemy : roomEnemies) {
			if (roomEnemy->GetPriority() == 1 ||
				(roomEnemy->GetPriority() == 2)) { //TODO fix the wellness threshold issues here!
				protectees.emplace_back(roomEnemy);
			}
		}

		if (protectees.size() == 0) {
			FocusTree();
		}
		else {
			//assign protectors to protectees
			//identify best moves for protectors (protectee follows behind protectors or focus biggest threat, 
			//     if able, put the protector between player and protectee)
		}
	}
}

void SwarmEngine::FocusTree()
{
	std::vector<AIEntity*> toAssignDirective = std::vector<AIEntity*>(roomEnemies);
	PlayerEntity* mostVaunerable = nullptr;
	
	for (PlayerEntity* player : players) {

		//Track most vaunerable player
		if (mostVaunerable == nullptr || player->GetWellness() < mostVaunerable->GetWellness()) {
			mostVaunerable = player;
		}

		// Is any type in hatred mode? (a player has killed a set amount of them?)
		//Y? go for that player, ties are broken by distance
	}
	
	//for every aientity that wasn't assigned yet
	for (AIEntity* entity : toAssignDirective) {
		AIDirective directive;

		//TODO fix this section
		if (entity->CanMove() && entity->GetWellness() > 50) {
			//go after most vaunerable player
			
			//directive->SetFocus(mostVaunerable);
		} else {
			PlayerEntity* player = FindClosestPlayer(entity->GetLocation());
			//directive->SetFocus(player);
		}

		entity->SetAIDirective(directive);
	}
}

void SwarmEngine::RemoveDeadEnemies()
{
	std::vector<AIEntity*> newRoomEnemies;

	std::copy_if(roomEnemies.begin(), roomEnemies.end(), std::back_inserter(newRoomEnemies), [](AIEntity* entity) { return entity->GetWellness() > 0; });

	roomEnemies.clear();

	roomEnemies.insert(newRoomEnemies.begin(), newRoomEnemies.end(), newRoomEnemies.end());
}

void SwarmEngine::RemoveUnavailablePlayers()
{
	//TODO -- remove players with 0 health or fully disconnected
	std::vector<PlayerEntity*> newPlayerList;

	std::copy_if(players.begin(), players.end(), std::back_inserter(newPlayerList), [](PlayerEntity* player) { return player->GetWellness() > 0; });

	players.clear();

	players.insert(newPlayerList.begin(), newPlayerList.end(), newPlayerList.end());
}

PlayerEntity * SwarmEngine::FindClosestPlayer(FVector location)
{
	PlayerEntity* closest = nullptr;
	float closestDist = -1;
	for (PlayerEntity* player : players) {
		float distance = FVector::Dist(location, player->GetLocation());

		if (!closest || distance < closestDist) {
			closest = player;
			closestDist = distance;
		}
	}
	return closest;
}
