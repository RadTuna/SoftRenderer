#pragma once

#include <memory>

#include "BaseComponent.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "ModelAsset.h"
#include "TextureAsset.h"


class RenderContext;
class RenderFactory;
class ModelRenderComponent : public BaseComponent
{
public:

	ModelRenderComponent(const std::shared_ptr<RenderContext>& InRenderer, const std::shared_ptr<RenderFactory>& InRenderFac);
	virtual ~ModelRenderComponent() = default;

	void Awake(Entity* InParent) override;
	void Update(float DeltaTime) override;
	void Render() override;
	void End() override;

	void GetWorldMatrix(Matrix4x4& OutMatrix);

private:

	std::unique_ptr<ModelAsset> mModelAsset;
	std::unique_ptr<TextureAsset> mAlbedoAsset;

	std::shared_ptr<RenderContext> mRenderer;
	std::shared_ptr<RenderFactory> mRenderFactory;

};
