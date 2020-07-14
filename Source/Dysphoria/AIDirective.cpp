// Copyright T Bilodeau 2020


#include "AIDirective.h"
#include "AIEntity.h"

AIDirective::AIDirective()
{
}

AIDirective::~AIDirective()
{
}

AIEntity* AIDirective::GetFocus() const
{
	return focusedEntity;
}

void AIDirective::SetFriendFocus(AIEntity * entity)
{
	focusedEntity = entity;
}

void AIDirective::SetPlayerFocus(PlayerEntity * entity)
{
	focusedPlayer = entity;
}

AIDirective::Directive AIDirective::GetDirective() const
{
	return directive;
}

void AIDirective::SetDirective(const Directive newDirective)
{
	directive = newDirective;
}
