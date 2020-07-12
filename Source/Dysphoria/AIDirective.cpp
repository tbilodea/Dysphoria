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

void AIDirective::SetFocus(AIEntity * entity)
{
	focusedEntity = entity;
}

AIDirective::Directive AIDirective::GetDirective() const
{
	return directive;
}

void AIDirective::SetDirective(const Directive newDirective)
{
	directive = newDirective;
}
