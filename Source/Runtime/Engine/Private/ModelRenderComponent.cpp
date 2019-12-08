
#include "Precompiled.h"
#include "..\Public\ModelRenderComponent.h"

#include "RenderFactory.h"
#include "RenderContext.h"
#include "Entity.h"

ModelRenderComponent::ModelRenderComponent(RenderContext* InRenderer, RenderFactory* InRenderFac)
	: mRenderer(InRenderer)
	, mRenderFactory(InRenderFac)
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
	assert(mRenderFactory);
	assert(mRenderer);

	VertexBuffer* RenderVertexBuffer = nullptr;
	mRenderFactory->CreateVertexBuffer(sizeof(VertexDataType) * mMeshLength, mMeshData.get(), &RenderVertexBuffer);

	IndexBuffer* RenderIndexBuffer = nullptr;
	mRenderFactory->CreateIndexBuffer(sizeof(UINT) * mIndexLength, mIndexData.get(), &RenderIndexBuffer);

	mRenderer->IASetVertexBuffer(RenderVertexBuffer, sizeof(VertexDataType));
	mRenderer->IASetIndexBuffer(RenderIndexBuffer, sizeof(UINT));
}

void ModelRenderComponent::End()
{
}

void ModelRenderComponent::SetMeshData(std::unique_ptr<VertexDataType[]>&& InMeshData, UINT InMeshLength)
{
	mMeshData = std::move(InMeshData);
	mMeshLength = InMeshLength;
}

void ModelRenderComponent::SetIndexData(std::unique_ptr<UINT[]>&& InIndexData, UINT InIndexLength)
{
	mIndexData = std::move(InIndexData);
	mIndexLength = InIndexLength;
}

void ModelRenderComponent::GetWorldMatrix(Matrix4x4& OutMatrix)
{
	OutMatrix.SetIdentity();

	OutMatrix = Matrix4x4::GetLocationMatrix(mParentEntity->GetLocation()) * OutMatrix;
	OutMatrix = Matrix4x4::GetRotationMatrix(mParentEntity->GetRotation()) * OutMatrix;
	OutMatrix = Matrix4x4::GetScaleMatrix(mParentEntity->GetScale()) * OutMatrix;
}
