#pragma once

#include <vector>
#include <memory>

#include "Entity.h"

class Scene final
{
public:

	Scene();
	~Scene();

	void Awake();
	void Update(float DeltaTime);
	void Render();
	void End();

	void AddEntity(std::unique_ptr<Entity>&& InEntity);

private:

	std::vector<std::unique_ptr<Entity>> Entities;

};
