
#include "Precompiled.h"
#include "..\Public\Entity.h"

void Entity::Awake()
{
	for (auto Iter = Components.begin(); Iter != Components.end(); ++Iter)
	{
		(*Iter)->Awake(this);
	}
}

void Entity::Update(float DeltaTime)
{
	for (auto Iter = Components.begin(); Iter != Components.end(); ++Iter)
	{
		(*Iter)->Update(DeltaTime);
	}
}

void Entity::Render()
{
	for (auto Iter = Components.begin(); Iter != Components.end(); ++Iter)
	{
		(*Iter)->Render();
	}
}

void Entity::End()
{
	for (auto Iter = Components.begin(); Iter != Components.end(); ++Iter)
	{
		(*Iter)->End();
	}
}

void Entity::AddComponent(std::unique_ptr<BaseComponent>&& InComp)
{
	Components.emplace_back(std::move(InComp));
}

void Entity::SetLocation(const Vector4& InLocation)
{
	Location = InLocation;
}

void Entity::SetRotation(const Vector3& InRotation)
{
	Rotation = InRotation;
}

void Entity::SetScale(const Vector3& InScale)
{
	Scale = InScale;
}
