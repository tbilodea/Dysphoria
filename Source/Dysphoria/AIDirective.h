// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include <memory>

class AIEntity;
class PlayerEntity;

/**
 * A directive for the AIEntity objects
 * 
 * Should have references for the logic of what the AIEntity should focus on
 */
class DYSPHORIA_API AIDirective
{
public:
	AIDirective();
	~AIDirective();

	UENUM()
	enum class Directive : uint8 {
		ATTACK,
		DEFEND,
		FLEE
	};

	AIEntity* GetFocus() const;
	void SetFriendFocus(AIEntity* entity);
	void SetPlayerFocus(PlayerEntity* player);

	Directive GetDirective() const;
	void SetDirective(const Directive directive);

private:
	Directive directive = Directive::ATTACK;
	AIEntity* focusedEntity = nullptr;
	PlayerEntity* focusedPlayer = nullptr;

};
