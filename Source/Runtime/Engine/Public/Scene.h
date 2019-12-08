#pragma once

#include <vector>
#include <memory>

#include "Entity.h"

class Scene final
{
public:

	Scene() = default;
	~Scene() = default;

	void Awake();
	void Update(float DeltaTime);
	void Render();
	void End();

	void AddEntity(std::unique_ptr<Entity>&& InEntity);

	Entity* GetEntity(const std::string& InTag, bool UseHash);
	Entity* GetEntity(std::size_t Index);

private:

	std::vector<std::unique_ptr<Entity>> mEntities;

public:

	FORCEINLINE const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return mEntities; }

};
