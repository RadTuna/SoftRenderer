
#include "Precompiled.h"
#include "..\Public\ModelRenderComponent.h"

#include "RenderFactory.h"
#include "RenderContext.h"
#include "Entity.h"

ModelRenderComponent::ModelRenderComponent(RenderContext* InRenderer, RenderFactory* InRenderFac)
	: Renderer(InRenderer)
	, RenderFac(InRenderFac)
{
}

void ModelRenderComponent::Awake(Entity* InParent)
{
	BaseComponent::Awake(InParent);
}

void ModelRenderComponent::Update(float DeltaTime)
{
}

void ModelRenderComponent::Render()
{
	assert(RenderFac);
	assert(Renderer);

	VertexBuffer* RenderVertexBuffer = nullptr;
	RenderFac->CreateVertexBuffer(sizeof(VertexDataType) * MeshLength, MeshData.get(), &RenderVertexBuffer);

	IndexBuffer* RenderIndexBuffer = nullptr;
	RenderFac->CreateIndexBuffer(sizeof(UINT) * IndexLength, IndexData.get(), &RenderIndexBuffer);

	Renderer->IASetVertexBuffer(RenderVertexBuffer, sizeof(VertexDataType));
	Renderer->IASetIndexBuffer(RenderIndexBuffer, sizeof(UINT));
}

void ModelRenderComponent::End()
{
}

void ModelRenderComponent::SetMeshData(std::unique_ptr<VertexDataType[]>&& InMeshData, UINT InMeshLength)
{
	MeshData = std::move(InMeshData);
	MeshLength = InMeshLength;
}

void ModelRenderComponent::SetIndexData(std::unique_ptr<UINT[]>&& InIndexData, UINT InIndexLength)
{
	IndexData = std::move(InIndexData);
	IndexLength = InIndexLength;
}

void ModelRenderComponent::GetWorldMatrix(Matrix4x4& OutMatrix)
{
	OutMatrix.SetIdentity();

	OutMatrix = Matrix4x4::GetLocationMatrix(ParentEntity->GetLocation()) * OutMatrix;
	OutMatrix = Matrix4x4::GetRotationMatrix(ParentEntity->GetRotation()) * OutMatrix;
	OutMatrix = Matrix4x4::GetScaleMatrix(ParentEntity->GetScale()) * OutMatrix;
}
