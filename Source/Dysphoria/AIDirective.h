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

	std::shared_ptr<AIEntity> GetFocus() const;
	void SetFocus(std::shared_ptr<AIEntity> entity);

private:
	Directive directive = Directive::ATTACK;
	std::shared_ptr<AIEntity> focusedEntity = nullptr;
};
