// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"

#include <map>
#include <vector>
#include <memory>

#include "EnemyClassType.h"
#include "SwarmEngine.generated.h"

class AAIEntity;
class APlayerEntity;

/**
 * The hive mind to figure out what each AIEntity needs to do to counter players
 */
UCLASS()
class DYSPHORIA_API USwarmEngine : public UObject
{
	GENERATED_BODY()

public:
	// Adds the list of players
	void AddPlayers(TArray<APlayerEntity*> AllPlayers);

	// Clears the old room enemies out and adds these new ones
	void AddNewRoomEnemies(TArray<AAIEntity*> AllRoomEnemies);

	// Add a single enemy (for ones that spawn others)
	void AddAdditionalRoomEnemy(AAIEntity* EnemyToAdd);

	// Runs the decision tree and updates the AIEntity statuses
	void RunEngine();

private:
	//Weights for determining how protectors are assigned
	const float DISTANCE_WEIGHT = 1.f;
	const float ASSIGNED_PROTECTORS_WEIGHT = 30.f;

	TArray<APlayerEntity*> Players = TArray<APlayerEntity*>();
	TArray<AAIEntity*> RoomEnemies = TArray<AAIEntity*>();

	void FocusTree(TArray<AAIEntity*>& ToAssignDirective, std::map<EEnemyType, TArray<APlayerEntity*>>& TypeToPlayersAboveThreshold);

	void RemoveDeadEnemies();
	void RemoveUnavailablePlayers();

	APlayerEntity* FindClosestPlayer(FVector Location);
	APlayerEntity* FindClosestPlayer(FVector Location, TArray<APlayerEntity*> PlayersToCheck);
	AAIEntity* FindClosestFriend(FVector Location);

	//Map EnemyType to the Players currently hated by that type
	std::map<EEnemyType, TArray<APlayerEntity*>> BuildHatedPlayerMap();

};
