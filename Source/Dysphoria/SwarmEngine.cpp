// Copyright T Bilodeau 2020


#include "SwarmEngine.h"

#include <limits>

#include "AIEntity.h"
#include "PlayerEntity.h"
#include "AIDirective.h"
#include "EnemyTypeUtils.h"
#include "EnemyData.h"

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
	roomEnemies.push_back(&enemyToAdd);
}

//Updates the AIDirectives for each entity
void SwarmEngine::RunEngine()
{
	RemoveDeadEnemies();
	RemoveUnavailablePlayers();

	std::map<EnemyType, std::vector<PlayerEntity*>> typeToPlayersAboveThreshold = BuildHatedPlayerMap();

	// Check number of AI
	if (roomEnemies.size() == 1) {
		FocusTree(roomEnemies, typeToPlayersAboveThreshold);
	}
	else {
		std::vector<AIEntity*> protectees = {};
		std::vector<AIEntity*> protectors = {};
		//Determine which have reached "protection level" which is priority 1 or priority 2 if the wellness hits a mark
		//(They should be generated in a kind of "order")
		for (auto& roomEnemy : roomEnemies) {
			if (roomEnemy->GetPriority() == 1) {
				protectees.insert(protectees.begin(), roomEnemy);
			}else if (roomEnemy->GetPriority() == 2 && roomEnemy->GetWellness() < 50) {
				protectees.push_back(roomEnemy);
			}
			else {
				protectors.push_back(roomEnemy);
			}
		}

		//If we're all protectors or protectees, just go for the focus
		if (protectees.size() == 0 || protectors.size() == 0) {
			FocusTree(roomEnemies, typeToPlayersAboveThreshold);
		}
		else {
			//We do the protector/protectee logic otherwise
			std::map<AIEntity*, std::vector<AIEntity*>> protecteesToProtectors;

			//A protectee will be assigned to find the nearest friend
			for (auto protectee : protectees) {
				AIDirective* directive = new AIDirective();
				directive->SetDirective(AIDirective::Directive::FLEE);
				directive->SetPlayerFocus(FindClosestPlayer(protectee->GetLocation()));
				directive->SetFriendFocus(FindClosestFriend(protectee->GetLocation()));

				protectee->SetAIDirective(directive);

				std::vector<AIEntity*> newVector = {};
				protecteesToProtectors.insert(std::make_pair(protectee, newVector));
			}

			//Protectors will be assigned based upon a weighted distance and amount of pre-assigned protectors
			for (auto protector : protectors) {
				AIEntity* minProtectee = nullptr;
				float minVal = std::numeric_limits<float>::max();
				for (auto protectee : protectees) {
					float dist = FVector::Dist(protectee->GetLocation(), protector->GetLocation());
					auto it = protecteesToProtectors.find(protectee);
					if (it != protecteesToProtectors.end()) {
						float amountOfProtectors = (float)(it->second).size();

						float weightedValue = dist * DISTANCE_WEIGHT + amountOfProtectors * ASSIGNED_PROTECTORS_WEIGHT;
						if (weightedValue < minVal) {
							minVal = weightedValue;
							minProtectee = protectee;
						}
					}
				}

				if (minProtectee) { continue; }

				//Add the protector to the map to be able to track how many each has
				auto it = protecteesToProtectors.find(minProtectee);
				if (it == protecteesToProtectors.end()) { continue; }
				(it->second).push_back(protector);

				//Make and assign the directive for the protector
				AIDirective* directive = new AIDirective();
				directive->SetFriendFocus(minProtectee);
				directive->SetPlayerFocus(FindClosestPlayer(protector->GetLocation()));
				directive->SetDirective(AIDirective::Directive::DEFEND);

				protector->SetAIDirective(directive);
			}
		}
	}
}

void SwarmEngine::FocusTree(std::vector<AIEntity*>& toAssignDirective, std::map<EnemyType, std::vector<PlayerEntity*>>& typeToPlayersAboveThreshold)
{
	PlayerEntity* mostVaunerable = nullptr;
	
	//Track most vaunerable player
	for (PlayerEntity* player : players) {
		if (!mostVaunerable || player->GetWellness() < mostVaunerable->GetWellness()) {
			mostVaunerable = player;
		}
	}

	//Assign the directives
	for (AIEntity* entity : toAssignDirective) {
		AIDirective* directive = new AIDirective();

		//Set directives for the hateful entities
		auto it = typeToPlayersAboveThreshold.find(entity->GetEnemyType());
		if (it != typeToPlayersAboveThreshold.end() && (it->second).size() != 0) {
			auto hatedPlayers = it->second;
			
			if (hatedPlayers.size() > 0) {
				PlayerEntity* closestHated = FindClosestPlayer(entity->GetLocation(), hatedPlayers);
				directive->SetPlayerFocus(closestHated);
				entity->SetAIDirective(directive);

				continue;
			}
		}
		
		//Default directives to the most vaunerable if entity is well, otherwise attack closest player
		if (entity->CanMove() && entity->GetWellness() > 50) {
			directive->SetPlayerFocus(mostVaunerable);
		} else {
			PlayerEntity* player = FindClosestPlayer(entity->GetLocation());
			directive->SetPlayerFocus(player);
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

PlayerEntity * SwarmEngine::FindClosestPlayer(FVector location) {
	return FindClosestPlayer(location, players);
}

PlayerEntity * SwarmEngine::FindClosestPlayer(FVector location, std::vector<PlayerEntity*> playersToCheck)
{
	PlayerEntity* closest = nullptr;
	float closestDist = -1;
	for (PlayerEntity* player : playersToCheck) {
		float distance = FVector::Dist(location, player->GetLocation());

		if (!closest || distance < closestDist) {
			closest = player;
			closestDist = distance;
		}
	}
	return closest;
}

AIEntity * SwarmEngine::FindClosestFriend(FVector location)
{
	AIEntity* closest = nullptr;
	float closestDist = -1;
	for (auto entity : roomEnemies) {
		float distance = FVector::Dist(location, entity->GetLocation());

		if (!closest || distance < closestDist) {
			closest = entity;
			closestDist = distance;
		}
	}
	return closest;
}

//Build map of EnemyType -> Players that are above the "hate" threshold
std::map<EnemyType, std::vector<PlayerEntity*>> SwarmEngine::BuildHatedPlayerMap() 
{
	std::map<EnemyType, std::vector<PlayerEntity*>> typeToPlayersAboveThreshold;
	for (auto type : EnemyTypeUtils::GetAllTypes()) {
		auto enemyData = EnemyTypeUtils::GetEnemyData(type);
		std::vector<PlayerEntity*> playersAboveKillThreshold = {};

		std::copy_if(players.begin(), players.end(), std::back_inserter(playersAboveKillThreshold),
			[&type, &enemyData](PlayerEntity* entity) {return entity->GetKillsOn(type) >= enemyData->GetHateLimit(); });

		typeToPlayersAboveThreshold.insert(std::make_pair(type, playersAboveKillThreshold));
	}
	return typeToPlayersAboveThreshold;
}
