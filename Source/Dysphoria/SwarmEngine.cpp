// Copyright T Bilodeau 2020


#include "SwarmEngine.h"

#include "AIEntity.h"
#include "PlayerEntity.h"
#include "SwarmDirective.h"
#include "EnemyTypeUtils.h"
#include "EnemyData.h"
#include "Dysphoria.h"

void USwarmEngine::AddPlayers(TArray<APlayerEntity*> AllPlayers)
{
	Players.Append(AllPlayers);
}

void USwarmEngine::AddNewRoomEnemies(TArray<AAIEntity*> NewRoomEnemies)
{
	RoomEnemies.Append(NewRoomEnemies);
}

void USwarmEngine::AddAdditionalRoomEnemy(AAIEntity* enemyToAdd)
{
	RoomEnemies.Add(enemyToAdd);
}

//Updates the AIDirectives for each entity
void USwarmEngine::RunEngine()
{
	RemoveDeadEnemies();
	RemoveUnavailablePlayers();

	if (Players.Num() == 0 || RoomEnemies.Num() == 0) { return; }

	std::map<EEnemyType, TArray<APlayerEntity*>> TypeToPlayersAboveThreshold = BuildHatedPlayerMap();

	UE_LOG(LogSwarmEngine, Log, TEXT("checking how many enemies"));
	
	// Check number of AI
	if (RoomEnemies.Num() == 1) {
		UE_LOG(LogSwarmEngine, Log, TEXT("Room enemies is 1"));
		FocusTree(RoomEnemies, TypeToPlayersAboveThreshold);
	}
	else {
		UE_LOG(LogSwarmEngine, Log, TEXT("Room enemies is above 1"));

		TArray<AAIEntity*> Protectees;
		TArray<AAIEntity*> Protectors;
		//Determine which have reached "protection level" which is priority 1 or priority 2 if the wellness hits a mark
		//(They should be generated in a kind of "order")
		for (auto& RoomEnemy : RoomEnemies) {
			if (RoomEnemy->GetPriority() == 1) {
				Protectees.Insert(RoomEnemy, 0);
				UE_LOG(LogSwarmEngine, Verbose, TEXT("%s is a high priority protectee"), *RoomEnemy->GetName());
			}else if (RoomEnemy->GetPriority() == 2 && RoomEnemy->GetWellness() < 50) {
				Protectees.Add(RoomEnemy);
				UE_LOG(LogSwarmEngine, Verbose, TEXT("%s is a protectee"), *RoomEnemy->GetName());
			}
			else {
				Protectors.Add(RoomEnemy);
				UE_LOG(LogSwarmEngine, Verbose, TEXT("%s is a protector"), *RoomEnemy->GetName());
			}
		}

		//If we're all protectors or protectees, just go for the focus
		if (Protectees.Num() == 0 || Protectors.Num() == 0) {
			UE_LOG(LogSwarmEngine, Log, TEXT("All protectees or protectors, using FocusTree"));
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
				UE_LOG(LogSwarmEngine, Log, TEXT("%s protectee entity [PlayerFocus of %s] [Friend to defend %s]"), *Protectee->GetName(), Directive->FocusedPlayer ? *Directive->FocusedPlayer->GetName() : *FString("null"), Directive->FocusedEntity ? *Directive->FocusedEntity->GetName() : *FString("null"));

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

				UE_LOG(LogSwarmEngine, Log, TEXT("%s protector entity has %s"), *Protector->GetName(), *Directive->ToString());
				Protector->SetAIDirective(Directive);
			}
		}
	}
}

void USwarmEngine::FocusTree(TArray<AAIEntity*>& ToAssignDirective, std::map<EEnemyType, TArray<APlayerEntity*>>& typeToPlayersAboveThreshold)
{
	APlayerEntity* MostVaunerable = nullptr;
	
	//Track most vaunerable player
	for (APlayerEntity* Player : Players) {
		if (!MostVaunerable || Player->GetWellness() < MostVaunerable->GetWellness()) {
			MostVaunerable = Player;
			UE_LOG(LogSwarmEngine, Log, TEXT("Current Most Vaunerable Player %s"), *(Player->GetName()));
		}
	}

	//Assign the directives
	for (AAIEntity* Entity : ToAssignDirective) {
		USwarmDirective * Directive = NewObject<USwarmDirective>();

		//Set directives for the hateful entities
		auto Iter = typeToPlayersAboveThreshold.find(Entity->GetEnemyType());
		if (Iter != typeToPlayersAboveThreshold.end() && (Iter->second).Num() != 0) {
			auto HatedPlayers = Iter->second;
			
			if (HatedPlayers.Num() > 0) {
				APlayerEntity* ClosestHated = FindClosestPlayer(Entity->GetLocation(), HatedPlayers);
				Directive->FocusedPlayer = ClosestHated;
				Entity->SetAIDirective(Directive);

				UE_LOG(LogSwarmEngine, Verbose, TEXT("Entity %s hates Player %s"), *Entity->GetName(), *ClosestHated->GetName());
				continue;
			}
		}
		
		//Default directives to the most vaunerable if entity is well, otherwise attack closest player
		if (Entity->GetCanMove() && Entity->GetWellness() > 50) {
			Directive->FocusedPlayer = MostVaunerable;
			UE_LOG(LogSwarmEngine, Verbose, TEXT("Assigning most vaunerable player %s"), *MostVaunerable->GetName(), *Entity->GetName());
		} else {
			APlayerEntity* Player = FindClosestPlayer(Entity->GetLocation());
			Directive->FocusedPlayer = Player;
			UE_LOG(LogSwarmEngine, Verbose, TEXT("Assigning closest player %s"), *Player->GetName());
		}

		UE_LOG(LogSwarmEngine, Log, TEXT("%s generic assigned entity has %s"), *Entity->GetName(), *Directive->ToString());

		Entity->SetAIDirective(Directive);
	}
}

void USwarmEngine::RemoveDeadEnemies()
{
	TArray<AAIEntity*> NewRoomEnemies = TArray<AAIEntity*>();

	for (auto& Entity : RoomEnemies) {
		if (Entity->GetWellness() > 0) {
			NewRoomEnemies.Add(Entity);
			UE_LOG(LogSwarmEngine, VeryVerbose, TEXT("Room Enemies list refreshed with %s"), *Entity->GetName());
		}
	}

	RoomEnemies.Empty();
	RoomEnemies.Append(NewRoomEnemies);
}

void USwarmEngine::RemoveUnavailablePlayers()
{
	//TODO -- remove players with 0 health or fully disconnected
	TArray<APlayerEntity*> NewPlayerList = TArray<APlayerEntity*>();

	for (auto& Player : Players) {
		if (Player->GetWellness() > 0) {
			NewPlayerList.Add(Player);
			UE_LOG(LogSwarmEngine, VeryVerbose, TEXT("Players list refreshed with %s"), *Player->GetName());
		}
	}

	Players.Empty();
	Players.Append(NewPlayerList);
}

APlayerEntity * USwarmEngine::FindClosestPlayer(FVector Location) {
	return FindClosestPlayer(Location, Players);
}

APlayerEntity * USwarmEngine::FindClosestPlayer(FVector Location, TArray<APlayerEntity*> PlayersToCheck)
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
	UE_LOG(LogSwarmEngine, VeryVerbose, TEXT("Closest player found is %s"), Closest ? *Closest->GetName() : *FString("null"));

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
	UE_LOG(LogSwarmEngine, VeryVerbose, TEXT("Closest Friend found is %s"), Closest ? *Closest->GetName() : *FString("null"));
	return Closest;
}

//Build map of EnemyType -> Players that are above the "hate" threshold
std::map<EEnemyType, TArray<APlayerEntity*>> USwarmEngine::BuildHatedPlayerMap()
{
	std::map<EEnemyType, TArray<APlayerEntity*>> TypeToPlayersAboveThreshold;
	for (auto Type : EnemyTypeUtils::GetAllTypes()) {
		auto EnemyData = EnemyTypeUtils::GetEnemyData(Type);
		TArray<APlayerEntity*> PlayersAboveKillThreshold = {};

		for (auto Player : Players) {
			if (Player->GetKillsOn(Type) >= EnemyData->GetHateLimit()) {
				PlayersAboveKillThreshold.Add(Player);
			}
		}
		
		TypeToPlayersAboveThreshold.insert(std::make_pair(Type, PlayersAboveKillThreshold));
	}
	UE_LOG(LogSwarmEngine, VeryVerbose, TEXT("Amount of players above hate threshold is %i"), TypeToPlayersAboveThreshold.size());
	return TypeToPlayersAboveThreshold;
}
