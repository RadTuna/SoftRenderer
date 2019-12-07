#pragma once

#include <vector>
#include <memory>
#include <string>

#include "BaseComponent.h"
#include "Vector4.h"
#include "Vector3.h"

class Entity final
{
public:

	Entity(const std::string& InTag);
	~Entity();

	void Awake();
	void Update(float DeltaTime);
	void Render();
	void End();

	void AddComponent(std::unique_ptr<BaseComponent>&& InComp);

	void SetLocation(const Vector4& InLocation);
	void SetRotation(const Vector3& InRotation);
	void SetScale(const Vector3& InScale);

	template<typename T>
	T* GetComponent();

private:

	std::vector<std::unique_ptr<BaseComponent>> Components;
	Vector4 Location;
	Vector3 Rotation;
	Vector3 Scale;
	std::string EntityTag;
	std::size_t HashCode;

public:

	FORCEINLINE const Vector4& GetLocation() const { return Location; }
	FORCEINLINE const Vector3& GetRotation() const { return Rotation; }
	FORCEINLINE const Vector3& GetScale() const { return Scale; }
	FORCEINLINE const std::string& GetTag() const { return EntityTag; }
	FORCEINLINE const std::size_t GetHash() const { return HashCode; }

};

template<typename T>
T* Entity::GetComponent()
{
	for (auto iter = Components.begin(); iter != Components.end(); ++iter)
	{
		if (typeid(T) == typeid(*(*iter).get()))
		{
			T* OutComponent = reinterpret_cast<T*>((*iter).get());
			return OutComponent;
		}
	}

	return nullptr;
}
