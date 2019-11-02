#pragma once

#define FORCEINLINE __forceinline
#define PRIMITIVE_COUNT 3

enum class BlendingModes
{
	Opaque,
	AlphaBlending
};

using UINT = unsigned __int32;

struct VertexBuffer
{
	unsigned char* Data;
	UINT DataSize;
};

struct IndexBuffer
{
	UINT* Data;
	UINT DataSize;
};
