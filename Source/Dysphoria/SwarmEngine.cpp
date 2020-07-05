// Copyright T Bilodeau 2020


#include "SwarmEngine.h"
#include "AIEntity.h"

SwarmEngine::SwarmEngine()
{
}

SwarmEngine::~SwarmEngine()
{
}

void SwarmEngine::AddPlayers(const std::vector<std::shared_ptr<PlayerEntity>>& allPlayers)
{
	players = allPlayers;
}

void SwarmEngine::AddNewRoomEnemies(const std::vector<std::shared_ptr<AIEntity>>& allRoomEnemies)
{
	roomEnemies = allRoomEnemies;
}

void SwarmEngine::AddAdditionalRoomEnemy(const std::shared_ptr<AIEntity>& enemyToAdd)
{
	roomEnemies.emplace_back(enemyToAdd);
}

//Updates the AIDirectives for each entity
void SwarmEngine::RunEngine()
{
	RemoveDeadEnemies();

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
	// Is any type in hatred mode? (a player has killed a set amount of them?)
	//Y? go for that player
	//else
	

	//if canmove and is well
	//go after most vaunerable player
	//else
	//go after closest
}

void SwarmEngine::RemoveDeadEnemies()
{
	//TODO check wellness. if 0 or below, remove it from the list
}
