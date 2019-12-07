#pragma once

#include <vector>
#include <memory>

#include "BaseComponent.h"
#include "Vector4.h"
#include "Vector3.h"

class Entity final
{
public:

	void Awake();
	void Update(float DeltaTime);
	void Render();
	void End();

	void AddComponent(std::unique_ptr<BaseComponent>&& InComp);

	void SetLocation(const Vector4& InLocation);
	void SetRotation(const Vector3& InRotation);
	void SetScale(const Vector3& InScale);

private:

	std::vector<std::unique_ptr<BaseComponent>> Components;
	Vector4 Location;
	Vector3 Rotation;
	Vector3 Scale;

public:

	FORCEINLINE const Vector4& GetLocation() const { return Location; }
	FORCEINLINE const Vector3& GetRotation() const { return Rotation; }
	FORCEINLINE const Vector3& GetScale() const { return Scale; }

};
