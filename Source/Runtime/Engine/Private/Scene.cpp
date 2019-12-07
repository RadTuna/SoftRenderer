
#include "Precompiled.h"
#include "..\Public\Scene.h"


void Scene::Awake()
{
	for (auto Iter = Entities.begin(); Iter != Entities.end(); ++Iter)
	{
		(*Iter)->Awake();
	}
}

void Scene::Update(float DeltaTime)
{
	for (auto Iter = Entities.begin(); Iter != Entities.end(); ++Iter)
	{
		(*Iter)->Update(DeltaTime);
	}
}

void Scene::Render()
{
	for (auto Iter = Entities.begin(); Iter != Entities.end(); ++Iter)
	{
		(*Iter)->Render();
	}
}

void Scene::End()
{
	for (auto Iter = Entities.begin(); Iter != Entities.end(); ++Iter)
	{
		(*Iter)->End();
	}
}

void Scene::AddEntity(std::unique_ptr<Entity>&& InEntity)
{
	Entities.emplace_back(std::move(InEntity));
}

Entity* Scene::GetEntity(const std::string& InTag, bool UseHash)
{
	std::size_t HashCode = 0;
	if (UseHash)
	{
		HashCode = std::hash<std::string>{}(InTag);
	}

	for (auto Iter = Entities.begin(); Iter != Entities.end(); ++Iter)
	{
		if (UseHash)
		{
			if (HashCode == (*Iter)->GetHash())
			{
				return (*Iter).get();
			}
		}
		else
		{
			if (InTag == (*Iter)->GetTag())
			{
				return (*Iter).get();
			}
		}
	}

	return nullptr;
}

Entity* Scene::GetEntity(std::size_t Index)
{
	if (Entities.size() >= Index || Index < 0)
	{
		return nullptr;
	}

	return Entities[Index].get();
}
