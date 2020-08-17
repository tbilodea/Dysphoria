#pragma once

#include "CoreMinimal.h"

#include "AIEntity.h"
#include "PlayerEntity.h"

#include "SwarmDirective.generated.h"


UENUM(BlueprintType)
enum class EDirective : uint8 {
	ATTACK UMETA(DisplayName = "Attack"),
	DEFEND UMETA(DisplayName = "Defend"),
	FLEE UMETA(DisplayName = "Flee")
};

UCLASS(BlueprintType)
class USwarmDirective : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDirective Directive = EDirective::ATTACK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AAIEntity* FocusedEntity = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerEntity* FocusedPlayer = nullptr;
};