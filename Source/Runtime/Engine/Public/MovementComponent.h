#pragma once

#include "BaseComponent.h"

class MovementComponent : public BaseComponent
{
public:

	MovementComponent() = default;
	virtual ~MovementComponent() = default;

	void Awake(class Entity* InParent) override;
	void Update(float DeltaTime) override;
	void Render() override;
	void End() override;

private:
};