#pragma once

#include <memory>
#include <vector>
#include <sstream>

#include "Asset.h"

struct VertexDataType
{
	Vector4 Position;
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

	std::unique_ptr<VertexDataType[]> Vertices;
	std::unique_ptr<UINT[]> Indices;
	UINT VerticesLength;
	UINT IndicesLength;

public:

	FORCEINLINE VertexDataType* GetVertices() const { return Vertices.get(); }
	FORCEINLINE UINT* GetIndices() const { return Indices.get(); }
	FORCEINLINE UINT GetVerticesLength() const { return VerticesLength; }
	FORCEINLINE UINT GetIndicesLength() const { return IndicesLength; }

};

