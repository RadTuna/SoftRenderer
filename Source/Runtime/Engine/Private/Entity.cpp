
#include "Precompiled.h"
#include "..\Public\Entity.h"

#include <functional>

Entity::Entity(const std::string& InTag)
	: mLocation(Vector4::Zero)
	, mRotation(Vector3::Zero)
	, mScale(Vector3::One)
	, mEntityTag(InTag)
{
	mHashCode = std::hash<std::string>{}(mEntityTag);
}

void Entity::Awake()
{
	for (auto Iter = mComponents.begin(); Iter != mComponents.end(); ++Iter)
	{
		(*Iter)->Awake(this);
	}
}

void Entity::Update(float DeltaTime)
{
	for (auto Iter = mComponents.begin(); Iter != mComponents.end(); ++Iter)
	{
		(*Iter)->Update(DeltaTime);
	}
}

void Entity::Render()
{
	for (auto Iter = mComponents.begin(); Iter != mComponents.end(); ++Iter)
	{
		(*Iter)->Render();
	}
}

void Entity::End()
{
	for (auto Iter = mComponents.begin(); Iter != mComponents.end(); ++Iter)
	{
		(*Iter)->End();
	}
}

void Entity::AddComponent(std::unique_ptr<BaseComponent>&& InComp)
{
	mComponents.emplace_back(std::move(InComp));
}

void Entity::Translate(const Vector4& InDeltaLocation)
{
	mLocation += InDeltaLocation;
}

void Entity::Rotate(const Vector3& InDeltaRotation)
{
	mRotation += InDeltaRotation;
}

void Entity::SetLocation(const Vector4& InLocation)
{
	mLocation = InLocation;
}

void Entity::SetRotation(const Vector3& InRotation)
{
	mRotation = InRotation;
}

void Entity::SetScale(const Vector3& InScale)
{
	mScale = InScale;
}
