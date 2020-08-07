// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwarmEngineManager.generated.h"

class SwarmEngine;

UCLASS(BlueprintType)
class DYSPHORIA_API ASwarmEngineManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwarmEngineManager();
	~ASwarmEngineManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPlayers();
	void AddRoomEnemies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	const float WAIT_TIME = 3000.f;

	SwarmEngine* SEngine = nullptr;
	float LastUpdateTime = 0.f;
	float AccumulatedTime = 0.f;
};
