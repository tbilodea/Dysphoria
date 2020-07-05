// Copyright T Bilodeau 2020


#include "AIEntity.h"

#include "AIDirective.h"


void AIEntity::SetAIDirective(const std::shared_ptr<AIDirective>& directive)
{
	currentDirective = directive;
}

int32 AIEntity::GetPriority() const
{
	return priority;
}

int32 AIEntity::GetWellness() const
{
	return wellness;
}

bool AIEntity::CanMove() const
{
	return canMove;
}

void AIEntity::Initialize()
{
	currentDirective = std::make_shared<AIDirective>(AIDirective());
}

void AIEntity::SetPriority(const int32 newPriority)
{
	priority = newPriority;
}

void AIEntity::SetWellness(const int32 newWellness)
{
	wellness = newWellness;
}

void AIEntity::SetCanMove(const bool newCanMove)
{
	canMove = newCanMove;
}

std::shared_ptr<AIDirective> AIEntity::GetAIDirective() const
{
	return currentDirective;
}

