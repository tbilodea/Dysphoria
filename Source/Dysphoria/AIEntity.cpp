// Copyright T Bilodeau 2020


#include "AIEntity.h"

#include "SwarmDirective.h"

int32 AIEntity::GetPriority() const
{
	return Priority;
}

int32 AIEntity::GetWellness() const
{
	return Wellness;
}

bool AIEntity::GetCanMove() const
{
	return CanMove;
}

AIEntity::~AIEntity()
{
	delete CurrentDirective;
}

void AIEntity::Initialize()
{
	FSwarmDirective* AiDirective, Dir;
	AiDirective = &Dir;
	CurrentDirective = AiDirective;
}

void AIEntity::SetPriority(const int32 NewPriority)
{
	Priority = NewPriority;
}

void AIEntity::SetWellness(const int32 NewWellness)
{
	Wellness = NewWellness;
}

void AIEntity::SetCanMove(const bool NewCanMove)
{
	CanMove = NewCanMove;
}

void AIEntity::SetAIDirective(FSwarmDirective* Directive)
{
	delete CurrentDirective;
	CurrentDirective = Directive;
}

FSwarmDirective AIEntity::GetAIDirective() const
{
	return *CurrentDirective;
}

