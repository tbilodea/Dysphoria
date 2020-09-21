// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIEntity.h"

#include "BaseAIEntity.generated.h"

enum class EEnemyType : uint8;

UCLASS()
class DYSPHORIA_API ABaseAIEntity : public AAIEntity
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAIEntity();

	//Swarm Engine methods
	FVector GetLocation();
	EEnemyType GetEnemyType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};