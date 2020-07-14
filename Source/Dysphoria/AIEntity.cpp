// Copyright T Bilodeau 2020


#include "AIEntity.h"

#include "AIDirective.h"

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

AIEntity::~AIEntity()
{
	delete currentDirective;
}

void AIEntity::Initialize()
{
	AIDirective* aiDirective = new AIDirective();
	currentDirective = aiDirective;
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

void AIEntity::SetAIDirective(AIDirective* directive)
{
	delete currentDirective;
	currentDirective = directive;
}

AIDirective AIEntity::GetAIDirective() const
{
	return *currentDirective;
}

