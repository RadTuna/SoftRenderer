#pragma once

#include <memory>

#include "BaseComponent.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4x4.h"

struct VertexDataType
{
	Vector4 Position;
	Vector3 Normal;
};

class ModelRenderComponent : public BaseComponent
{
public:

	ModelRenderComponent(class RenderContext* InRenderer, class RenderFactory* InRenderFac);
	virtual ~ModelRenderComponent() = default;

	void Awake(Entity* InParent) override;
	void Update(float DeltaTime) override;
	void Render() override;
	void End() override;

	void SetMeshData(std::unique_ptr<VertexDataType[]>&& InMeshData, UINT InMeshLength);
	void SetIndexData(std::unique_ptr<UINT[]>&& InIndexData, UINT InIndexLength);

	void GetWorldMatrix(Matrix4x4& OutMatrix);

private:

	std::unique_ptr<VertexDataType[]> MeshData;
	std::unique_ptr<UINT[]> IndexData;
	UINT MeshLength;
	UINT IndexLength;
	class RenderContext* Renderer;
	class RenderFactory* RenderFac;

};
