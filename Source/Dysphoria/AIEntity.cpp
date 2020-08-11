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

AAIEntity::~AAIEntity()
{
	delete CurrentDirective;
}

void AAIEntity::Initialize()
{
	FSwarmDirective* AiDirective, Dir;
	AiDirective = &Dir;
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

void AAIEntity::SetAIDirective(FSwarmDirective* Directive)
{
	delete CurrentDirective;
	CurrentDirective = Directive;
}

FSwarmDirective AAIEntity::GetAIDirective() const
{
	return *CurrentDirective;
}

