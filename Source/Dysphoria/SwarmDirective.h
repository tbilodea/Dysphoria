#pragma

#include "CoreMinimal.h"

#include "AIEntity.h"
#include "PlayerEntity.h"

#include "SwarmDirective.generated.h"


UENUM(BlueprintType)
enum class EDirective : uint8 {
	ATTACK,
	DEFEND,
	FLEE
};

USTRUCT(BlueprintType)
struct FSwarmDirective {
	GENERATED_BODY()

	UPROPERTY()
	EDirective Directive = EDirective::ATTACK;

	UPROPERTY()
	AAIEntity* FocusedEntity = nullptr;

	UPROPERTY()
	APlayerEntity* FocusedPlayer = nullptr;
};