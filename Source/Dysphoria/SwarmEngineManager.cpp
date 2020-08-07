// Copyright T Bilodeau 2020


#include "SwarmEngineManager.h"
#include "Kismet/GameplayStatics.h" 
#include "SwarmEngine.h"
#include "FirstPersonController.h"
#include "BaseAIEntity.h"
#include "AIEntity.h"

// Sets default values
ASwarmEngineManager::ASwarmEngineManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SEngine = new SwarmEngine();
}

ASwarmEngineManager::~ASwarmEngineManager() {
	delete SEngine;
}

// Called when the game starts or when spawned
void ASwarmEngineManager::BeginPlay()
{
	Super::BeginPlay();

	AddPlayers();
	AddRoomEnemies();
}

// Called every frame
void ASwarmEngineManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AccumulatedTime += DeltaTime;

	if (AccumulatedTime > WAIT_TIME) {
		UE_LOG(LogTemp, Warning, TEXT("Running SwarmEngine"));

		AccumulatedTime = 0.f;

		SEngine->RunEngine();
	}
}

void ASwarmEngineManager::AddPlayers() {

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFirstPersonController::StaticClass(), FoundActors);
	std::vector<PlayerEntity*> allPlayers = {};
	for (auto Actor : FoundActors) {
		allPlayers.push_back(static_cast<AFirstPersonController*>(Actor));
	}	
	SEngine->AddPlayers(allPlayers);
}

void ASwarmEngineManager::AddRoomEnemies() {
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseAIEntity::StaticClass(), FoundActors);
	std::vector<AIEntity*> enemies = {};
	for (auto Enemy : FoundActors) {
		enemies.push_back((AIEntity*) static_cast<ABaseAIEntity*>(Enemy));
	}
	SEngine->AddNewRoomEnemies(enemies);
}