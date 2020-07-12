// Copyright T Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include <memory>

class AIEntity;

/**
 * A directive for the AIEntity objects
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
	void SetFocus(AIEntity* entity);

	Directive GetDirective() const;
	void SetDirective(const Directive directive);

private:
	Directive directive = Directive::ATTACK;
	AIEntity* focusedEntity = nullptr;
};
