
#include "Precompiled.h"
#include "..\Public\Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

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
