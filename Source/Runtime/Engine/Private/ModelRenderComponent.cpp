
#include "Precompiled.h"
#include "..\Public\ModelRenderComponent.h"

#include "Entity.h"
#include "RenderFactory.h"
#include "RenderContext.h"


ModelRenderComponent::ModelRenderComponent(const std::shared_ptr<RenderContext>& InRenderer, const std::shared_ptr<RenderFactory>& InRenderFac)
	: mRenderer(InRenderer)
	, mRenderFactory(InRenderFac)
{
	mModelAsset = std::make_unique<ModelAsset>();
}

void ModelRenderComponent::Awake(Entity* InParent)
{
	BaseComponent::Awake(InParent);

	mModelAsset->Load("../Resource/Sphere.obj");
}

void ModelRenderComponent::Update(float DeltaTime)
{
}

void ModelRenderComponent::Render()
{
	assert(mRenderFactory);
	assert(mRenderer);

	VertexBuffer* RenderVertexBuffer = nullptr;
	mRenderFactory->CreateVertexBuffer(sizeof(VertexDataType) * mModelAsset->GetVerticesLength(), mModelAsset->GetVertices(), &RenderVertexBuffer);

	IndexBuffer* RenderIndexBuffer = nullptr;
	mRenderFactory->CreateIndexBuffer(sizeof(UINT) * mModelAsset->GetIndicesLength(), mModelAsset->GetIndices(), &RenderIndexBuffer);

	mRenderer->IASetVertexBuffer(RenderVertexBuffer, sizeof(VertexDataType));
	mRenderer->IASetIndexBuffer(RenderIndexBuffer, sizeof(UINT));
}

void ModelRenderComponent::End()
{
}

void ModelRenderComponent::GetWorldMatrix(Matrix4x4& OutMatrix)
{
	OutMatrix.SetIdentity();

	OutMatrix = Matrix4x4::GetLocationMatrix(mParentEntity->GetLocation()) * OutMatrix;
	OutMatrix = Matrix4x4::GetRotationMatrix(mParentEntity->GetRotation()) * OutMatrix;
	OutMatrix = Matrix4x4::GetScaleMatrix(mParentEntity->GetScale()) * OutMatrix;
}
