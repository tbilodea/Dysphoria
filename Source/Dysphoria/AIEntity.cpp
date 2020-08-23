// Copyright T Bilodeau 2020


#include "AIEntity.h"
#include "SwarmDirective.h"

int32 AAIEntity::GetPriority() const
{
	return Priority;
}

int32 AAIEntity::GetWellness() const
{
	return Wellness;
}

bool AAIEntity::GetCanMove() const
{
	return CanMove;
}

void AAIEntity::Initialize()
{
	USwarmDirective* AiDirective = NewObject<USwarmDirective>();
	CurrentDirective = AiDirective;
}

void AAIEntity::SetPriority(const int32 NewPriority)
{
	Priority = NewPriority;
}

void AAIEntity::SetWellness(const int32 NewWellness)
{
	Wellness = NewWellness;
}

void AAIEntity::SetCanMove(const bool NewCanMove)
{
	CanMove = NewCanMove;
}

void AAIEntity::SetAIDirective(USwarmDirective* Directive)
{
	CurrentDirective = Directive;
}

USwarmDirective* AAIEntity::GetAIDirective()
{
	return CurrentDirective;
}

