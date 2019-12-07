#include "Precompiled.h"
#include "..\Public\BaseComponent.h"

#include <assert.h>

#include "Entity.h"

void BaseComponent::Awake(Entity* InParent)
{
	ParentEntity = InParent;
	assert(ParentEntity);
}
