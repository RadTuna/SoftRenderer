#pragma once

#define FORCEINLINE __forceinline

#define PRIMITIVE_COUNT 3
#define PRIMITIVE_INDEX_ONE 0
#define PRIMITIVE_INDEX_TWO 1
#define PRIMITIVE_INDEX_THREE 2

enum class BlendingModes
{
	Opaque,
	AlphaBlending
};

enum class CullingMode
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK
};

using UINT = unsigned __int32;

struct VertexBuffer
{
	unsigned char* Data;
	UINT DataSize;
};

struct IndexBuffer
{
	unsigned char* Data;
	UINT DataSize;
};

class LinearColor;
struct RenderTexture
{
	LinearColor* ColorData;
	UINT DataSize;
	UINT TexHeight;
	UINT TexWidth;
};
