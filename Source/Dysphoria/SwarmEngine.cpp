// Copyright T Bilodeau 2020


#include "SwarmEngine.h"

#include <limits>

#include "AIEntity.h"
#include "PlayerEntity.h"
#include "SwarmDirective.h"
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
	Players.clear();

	Players.insert(allPlayers.begin(), allPlayers.end(), allPlayers.end());
}

void SwarmEngine::AddNewRoomEnemies(const std::vector<AIEntity*>& newRoomEnemies)
{
	RoomEnemies.clear();

	RoomEnemies.insert(newRoomEnemies.begin(), newRoomEnemies.end(), newRoomEnemies.end());
}

void SwarmEngine::AddAdditionalRoomEnemy(AIEntity& enemyToAdd)
{
	RoomEnemies.push_back(&enemyToAdd);
}

//Updates the AIDirectives for each entity
void SwarmEngine::RunEngine()
{
	RemoveDeadEnemies();
	RemoveUnavailablePlayers();

	std::map<EnemyType, std::vector<PlayerEntity*>> TypeToPlayersAboveThreshold = BuildHatedPlayerMap();

	// Check number of AI
	if (RoomEnemies.size() == 1) {
		FocusTree(RoomEnemies, TypeToPlayersAboveThreshold);
	}
	else {
		std::vector<AIEntity*> Protectees = {};
		std::vector<AIEntity*> Protectors = {};
		//Determine which have reached "protection level" which is priority 1 or priority 2 if the wellness hits a mark
		//(They should be generated in a kind of "order")
		for (auto& RoomEnemy : RoomEnemies) {
			if (RoomEnemy->GetPriority() == 1) {
				Protectees.insert(Protectees.begin(), RoomEnemy);
			}else if (RoomEnemy->GetPriority() == 2 && RoomEnemy->GetWellness() < 50) {
				Protectees.push_back(RoomEnemy);
			}
			else {
				Protectors.push_back(RoomEnemy);
			}
		}

		//If we're all protectors or protectees, just go for the focus
		if (Protectees.size() == 0 || Protectors.size() == 0) {
			FocusTree(RoomEnemies, TypeToPlayersAboveThreshold);
		}
		else {
			//We do the protector/protectee logic otherwise
			std::map<AIEntity*, std::vector<AIEntity*>> ProtecteesToProtectors;

			//A protectee will be assigned to find the nearest friend
			for (auto Protectee : Protectees) {
				FSwarmDirective *Directive, Dir;
				Directive = &Dir;
				Directive->Directive = EDirective::FLEE;
				Directive->FocusedPlayer = FindClosestPlayer(Protectee->GetLocation());
				Directive->FocusedEntity = FindClosestFriend(Protectee->GetLocation());

				Protectee->SetAIDirective(Directive);

				std::vector<AIEntity*> newVector = {};
				ProtecteesToProtectors.insert(std::make_pair(Protectee, newVector));
			}

			//Protectors will be assigned based upon a weighted distance and amount of pre-assigned protectors
			for (auto Protector : Protectors) {
				AIEntity* MinProtectee = nullptr;
				float MinVal = std::numeric_limits<float>::max();
				for (auto Protectee : Protectees) {
					float Dist = FVector::Dist(Protectee->GetLocation(), Protector->GetLocation());
					auto Iter = ProtecteesToProtectors.find(Protectee);
					if (Iter != ProtecteesToProtectors.end()) {
						float AmountOfProtectors = (float)(Iter->second).size();

						float WeightedValue = Dist * DISTANCE_WEIGHT + AmountOfProtectors * ASSIGNED_PROTECTORS_WEIGHT;
						if (WeightedValue < MinVal) {
							MinVal = WeightedValue;
							MinProtectee = Protectee;
						}
					}
				}

				if (MinProtectee) { continue; }

				//Add the protector to the map to be able to track how many each has
				auto it = ProtecteesToProtectors.find(MinProtectee);
				if (it == ProtecteesToProtectors.end()) { continue; }
				(it->second).push_back(Protector);

				//Make and assign the directive for the protector
				FSwarmDirective *Directive, Dir;
				Directive = &Dir;
				Directive->FocusedEntity = MinProtectee;
				Directive->FocusedPlayer = FindClosestPlayer(Protector->GetLocation());
				Directive->Directive = EDirective::DEFEND;

				Protector->SetAIDirective(Directive);
			}
		}
	}
}

void SwarmEngine::FocusTree(std::vector<AIEntity*>& ToAssignDirective, std::map<EnemyType, std::vector<PlayerEntity*>>& typeToPlayersAboveThreshold)
{
	PlayerEntity* MostVaunerable = nullptr;
	
	//Track most vaunerable player
	for (PlayerEntity* Player : Players) {
		if (!MostVaunerable || Player->GetWellness() < MostVaunerable->GetWellness()) {
			MostVaunerable = Player;
		}
	}

	//Assign the directives
	for (AIEntity* Entity : ToAssignDirective) {
		FSwarmDirective *Directive, Dir;
		Directive = &Dir;

		//Set directives for the hateful entities
		auto Iter = typeToPlayersAboveThreshold.find(Entity->GetEnemyType());
		if (Iter != typeToPlayersAboveThreshold.end() && (Iter->second).size() != 0) {
			auto HatedPlayers = Iter->second;
			
			if (HatedPlayers.size() > 0) {
				PlayerEntity* ClosestHated = FindClosestPlayer(Entity->GetLocation(), HatedPlayers);
				Directive->FocusedPlayer = ClosestHated;
				Entity->SetAIDirective(Directive);

				continue;
			}
		}
		
		//Default directives to the most vaunerable if entity is well, otherwise attack closest player
		if (Entity->GetCanMove() && Entity->GetWellness() > 50) {
			Directive->FocusedPlayer = MostVaunerable;
		} else {
			PlayerEntity* Player = FindClosestPlayer(Entity->GetLocation());
			Directive->FocusedPlayer = Player;
		}

		Entity->SetAIDirective(Directive);
	}
}

void SwarmEngine::RemoveDeadEnemies()
{
	std::vector<AIEntity*> NewRoomEnemies;

	std::copy_if(RoomEnemies.begin(), RoomEnemies.end(), std::back_inserter(NewRoomEnemies), [](AIEntity* Entity) { return Entity->GetWellness() > 0; });

	RoomEnemies.clear();

	RoomEnemies.insert(NewRoomEnemies.begin(), NewRoomEnemies.end(), NewRoomEnemies.end());
}

void SwarmEngine::RemoveUnavailablePlayers()
{
	//TODO -- remove players with 0 health or fully disconnected
	std::vector<PlayerEntity*> NewPlayerList;

	std::copy_if(Players.begin(), Players.end(), std::back_inserter(NewPlayerList), [](PlayerEntity* Player) { return Player->GetWellness() > 0; });

	Players.clear();

	Players.insert(NewPlayerList.begin(), NewPlayerList.end(), NewPlayerList.end());
}

PlayerEntity * SwarmEngine::FindClosestPlayer(FVector Location) {
	return FindClosestPlayer(Location, Players);
}

PlayerEntity * SwarmEngine::FindClosestPlayer(FVector Location, std::vector<PlayerEntity*> PlayersToCheck)
{
	PlayerEntity* Closest = nullptr;
	float ClosestDist = -1;
	for (PlayerEntity* Player : PlayersToCheck) {
		float Distance = FVector::Dist(Location, Player->GetLocation());

		if (!Closest || Distance < ClosestDist) {
			Closest = Player;
			ClosestDist = Distance;
		}
	}
	return Closest;
}

AIEntity * SwarmEngine::FindClosestFriend(FVector Location)
{
	AIEntity* Closest = nullptr;
	float ClosestDist = -1;
	for (auto Entity : RoomEnemies) {
		float Distance = FVector::Dist(Location, Entity->GetLocation());

		if (!Closest || Distance < ClosestDist) {
			Closest = Entity;
			ClosestDist = Distance;
		}
	}
	return Closest;
}

//Build map of EnemyType -> Players that are above the "hate" threshold
std::map<EnemyType, std::vector<PlayerEntity*>> SwarmEngine::BuildHatedPlayerMap() 
{
	std::map<EnemyType, std::vector<PlayerEntity*>> TypeToPlayersAboveThreshold;
	for (auto Type : EnemyTypeUtils::GetAllTypes()) {
		auto EnemyData = EnemyTypeUtils::GetEnemyData(Type);
		std::vector<PlayerEntity*> PlayersAboveKillThreshold = {};

		std::copy_if(Players.begin(), Players.end(), std::back_inserter(PlayersAboveKillThreshold),
			[&Type, &EnemyData](PlayerEntity* Entity) {return Entity->GetKillsOn(Type) >= EnemyData->GetHateLimit(); });

		TypeToPlayersAboveThreshold.insert(std::make_pair(Type, PlayersAboveKillThreshold));
	}
	return TypeToPlayersAboveThreshold;
}
