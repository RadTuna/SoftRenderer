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
	~Entity() = default;

	void Awake();
	void Update(float DeltaTime);
	void Render();
	void End();

	void AddComponent(std::unique_ptr<BaseComponent>&& InComp);

	void Translate(const Vector4& InDeltaLocation);
	void Rotate(const Vector3& InDeltaRotation);

	void SetLocation(const Vector4& InLocation);
	void SetRotation(const Vector3& InRotation);
	void SetScale(const Vector3& InScale);

	template<typename T>
	T* GetComponent();

private:

	std::vector<std::unique_ptr<BaseComponent>> mComponents;
	Vector4 mLocation;
	Vector3 mRotation;
	Vector3 mScale;
	std::string mEntityTag;
	std::size_t mHashCode;

public:

	FORCEINLINE const Vector4& GetLocation() const { return mLocation; }
	FORCEINLINE const Vector3& GetRotation() const { return mRotation; }
	FORCEINLINE const Vector3& GetScale() const { return mScale; }
	FORCEINLINE const std::string& GetTag() const { return mEntityTag; }
	FORCEINLINE const std::size_t GetHash() const { return mHashCode; }

};

template<typename T>
T* Entity::GetComponent()
{
	for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
	{
		if (typeid(T) == typeid(*(*iter).get()))
		{
			T* OutComponent = reinterpret_cast<T*>((*iter).get());
			return OutComponent;
		}
	}

	return nullptr;
}
