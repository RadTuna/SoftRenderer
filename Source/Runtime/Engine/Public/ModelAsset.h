#pragma once

#include <memory>
#include <vector>
#include <sstream>

#include "Asset.h"

struct VertexDataType
{
	Vector4 Position;
	Vector2 UV;
	Vector3 Normal;
};

// Use OBJ File
class ModelAsset final : public Asset
{
public:

	explicit ModelAsset() noexcept = default;
	virtual ~ModelAsset() noexcept = default;

	// without IBO
	void Load(const std::string& InPath) override;

private:

	std::unique_ptr<VertexDataType[]> mVertices;
	std::unique_ptr<UINT[]> mIndices;
	UINT mVerticesLength;
	UINT mIndicesLength;

public:

	FORCEINLINE VertexDataType* GetVertices() const { return mVertices.get(); }
	FORCEINLINE UINT* GetIndices() const { return mIndices.get(); }
	FORCEINLINE UINT GetVerticesLength() const { return mVerticesLength; }
	FORCEINLINE UINT GetIndicesLength() const { return mIndicesLength; }

};

