// Copyright T Bilodeau 2020


#include "SwarmEngine.h"

#include <limits>
#include <algorithm>
#include <iterator>

#include "AIEntity.h"
#include "PlayerEntity.h"
#include "SwarmDirective.h"
#include "EnemyTypeUtils.h"
#include "EnemyData.h"

void USwarmEngine::AddPlayers(const std::vector<APlayerEntity*>& allPlayers)
{
	Players.clear();

	Players.insert(allPlayers.begin(), allPlayers.end(), allPlayers.end());
}

void USwarmEngine::AddNewRoomEnemies(const std::vector<AAIEntity*>& NewRoomEnemies)
{
	RoomEnemies.clear();

	RoomEnemies.insert(NewRoomEnemies.begin(), NewRoomEnemies.end(), NewRoomEnemies.end());
}

void USwarmEngine::AddAdditionalRoomEnemy(AAIEntity& enemyToAdd)
{
	RoomEnemies.push_back(&enemyToAdd);
}

//Updates the AIDirectives for each entity
void USwarmEngine::RunEngine()
{
	RemoveDeadEnemies();
	RemoveUnavailablePlayers();

	std::map<EEnemyType, std::vector<APlayerEntity*>> TypeToPlayersAboveThreshold = BuildHatedPlayerMap();

	UE_LOG(LogTemp, Warning, TEXT("checking how many enemies"));
	
	// Check number of AI
	if (RoomEnemies.size() == 1) {
		UE_LOG(LogTemp, Warning, TEXT("room enemies is 1"));
		FocusTree(RoomEnemies, TypeToPlayersAboveThreshold);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("room enemies is above 1"));

		std::vector<AAIEntity*> Protectees = {};
		std::vector<AAIEntity*> Protectors = {};
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
			std::map<AAIEntity*, std::vector<AAIEntity*>> ProtecteesToProtectors;

			//A protectee will be assigned to find the nearest friend
			for (auto Protectee : Protectees) {
				USwarmDirective * Directive = NewObject<USwarmDirective>();
				Directive->Directive = EDirective::FLEE;
				Directive->FocusedPlayer = FindClosestPlayer(Protectee->GetLocation());
				Directive->FocusedEntity = FindClosestFriend(Protectee->GetLocation());

				Protectee->SetAIDirective(Directive);

				std::vector<AAIEntity*> newVector = {};
				ProtecteesToProtectors.insert(std::make_pair(Protectee, newVector));
			}

			//Protectors will be assigned based upon a weighted distance and amount of pre-assigned protectors
			for (auto Protector : Protectors) {
				AAIEntity* MinProtectee = nullptr;
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
				USwarmDirective * Directive = NewObject<USwarmDirective>();
				Directive->FocusedEntity = MinProtectee;
				Directive->FocusedPlayer = FindClosestPlayer(Protector->GetLocation());
				Directive->Directive = EDirective::DEFEND;
				
				UE_LOG(LogTemp, Warning, TEXT("Defending player closest is %s"), *(Directive->FocusedPlayer->GetName()));

				Protector->SetAIDirective(Directive);
			}
		}
	}
}

void USwarmEngine::FocusTree(std::vector<AAIEntity*>& ToAssignDirective, std::map<EEnemyType, std::vector<APlayerEntity*>>& typeToPlayersAboveThreshold)
{
	APlayerEntity* MostVaunerable = nullptr;
	
	//Track most vaunerable player
	for (APlayerEntity* Player : Players) {
		if (!MostVaunerable || Player->GetWellness() < MostVaunerable->GetWellness()) {
			MostVaunerable = Player;
		}
	}

	//Assign the directives
	for (AAIEntity* Entity : ToAssignDirective) {
		USwarmDirective * Directive = NewObject<USwarmDirective>();

		//Set directives for the hateful entities
		auto Iter = typeToPlayersAboveThreshold.find(Entity->GetEnemyType());
		if (Iter != typeToPlayersAboveThreshold.end() && (Iter->second).size() != 0) {
			auto HatedPlayers = Iter->second;
			
			if (HatedPlayers.size() > 0) {
				APlayerEntity* ClosestHated = FindClosestPlayer(Entity->GetLocation(), HatedPlayers);
				Directive->FocusedPlayer = ClosestHated;
				Entity->SetAIDirective(Directive);

				continue;
			}
		}
		
		//Default directives to the most vaunerable if entity is well, otherwise attack closest player
		if (Entity->GetCanMove() && Entity->GetWellness() > 50) {
			Directive->FocusedPlayer = MostVaunerable;
		} else {
			APlayerEntity* Player = FindClosestPlayer(Entity->GetLocation());
			Directive->FocusedPlayer = Player;
		}

		Entity->SetAIDirective(Directive);

		if (Directive->FocusedPlayer) {
			UE_LOG(LogTemp, Warning, TEXT("focusing a player %s"), *(Directive->FocusedPlayer->GetName()));
		}
		UE_LOG(LogTemp, Warning, TEXT("focus a player called"));
	}
}

void USwarmEngine::RemoveDeadEnemies()
{
	std::vector<AAIEntity*> NewRoomEnemies;

	std::copy_if(RoomEnemies.begin(), RoomEnemies.end(), std::back_inserter(NewRoomEnemies), [](AAIEntity* Entity) { return Entity->GetWellness() > 0; });

	RoomEnemies.clear();

	RoomEnemies = NewRoomEnemies;
}

void USwarmEngine::RemoveUnavailablePlayers()
{
	//TODO -- remove players with 0 health or fully disconnected
	std::vector<APlayerEntity*> NewPlayerList;

	std::copy_if(Players.begin(), Players.end(), std::back_inserter(NewPlayerList), [](APlayerEntity* Player) { return Player->GetWellness() > 0; });

	Players.clear();

	Players.insert(NewPlayerList.begin(), NewPlayerList.end(), NewPlayerList.end());
}

APlayerEntity * USwarmEngine::FindClosestPlayer(FVector Location) {
	return FindClosestPlayer(Location, Players);
}

APlayerEntity * USwarmEngine::FindClosestPlayer(FVector Location, std::vector<APlayerEntity*> PlayersToCheck)
{
	APlayerEntity* Closest = nullptr;
	float ClosestDist = -1;
	for (APlayerEntity* Player : PlayersToCheck) {
		float Distance = FVector::Dist(Location, Player->GetLocation());

		if (!Closest || Distance < ClosestDist) {
			Closest = Player;
			ClosestDist = Distance;
		}
	}
	return Closest;
}

AAIEntity * USwarmEngine::FindClosestFriend(FVector Location)
{
	AAIEntity* Closest = nullptr;
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
std::map<EEnemyType, std::vector<APlayerEntity*>> USwarmEngine::BuildHatedPlayerMap() 
{
	std::map<EEnemyType, std::vector<APlayerEntity*>> TypeToPlayersAboveThreshold;
	for (auto Type : EnemyTypeUtils::GetAllTypes()) {
		auto EnemyData = EnemyTypeUtils::GetEnemyData(Type);
		std::vector<APlayerEntity*> PlayersAboveKillThreshold = {};

		std::copy_if(Players.begin(), Players.end(), std::back_inserter(PlayersAboveKillThreshold),
			[&Type, &EnemyData](APlayerEntity* Entity) {return Entity->GetKillsOn(Type) >= EnemyData->GetHateLimit(); });

		TypeToPlayersAboveThreshold.insert(std::make_pair(Type, PlayersAboveKillThreshold));
	}
	return TypeToPlayersAboveThreshold;
}
