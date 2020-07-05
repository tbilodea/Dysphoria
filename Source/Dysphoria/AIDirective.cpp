// Copyright T Bilodeau 2020


#include "AIDirective.h"
#include "AIEntity.h"

AIDirective::AIDirective()
{
}

AIDirective::~AIDirective()
{
}

std::shared_ptr<AIEntity> AIDirective::GetFocus() const
{
	return focusedEntity;
}

void AIDirective::SetFocus(std::shared_ptr<AIEntity> entity)
{
	focusedEntity = entity;
}