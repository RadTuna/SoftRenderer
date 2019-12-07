#pragma once

class BaseComponent
{
public:

	BaseComponent() = default;
	virtual ~BaseComponent() = default;

	virtual void Awake(class Entity* InParent);
	virtual void Update(float DeltaTime) = 0;
	virtual void Render() = 0;
	virtual void End() = 0;

protected:

	class Entity* ParentEntity;

};
