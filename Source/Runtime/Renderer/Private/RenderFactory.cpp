
#include "Precompiled.h"
#include "..\Public\RenderFactory.h"

bool RenderFactory::CreateVertexBuffer(UINT DataSize, void* InData, VertexBuffer** OutBuffer)
{
	if (InData == nullptr || *OutBuffer != nullptr)
	{
		return false;
	}

	unsigned char* InnerData = reinterpret_cast<unsigned char*>(InData);
	if (InnerData == nullptr)
	{
		return false;
	}

	(*OutBuffer) = new VertexBuffer;
	if (OutBuffer == nullptr)
	{
		return false;
	}

	(*OutBuffer)->Data = new unsigned char[DataSize];
	if ((*OutBuffer)->Data == nullptr)
	{
		delete OutBuffer;
		OutBuffer = nullptr;
		return false;
	}

	for (UINT i = 0; i < DataSize; ++i)
	{
		(*OutBuffer)->Data[i] = InnerData[i];
	}
	(*OutBuffer)->DataSize = DataSize;

	return true;
}

bool RenderFactory::CreateIndexBuffer(UINT DataSize, void* InData, IndexBuffer** OutBuffer)
{
	if (InData == nullptr || *OutBuffer != nullptr)
	{
		return false;
	}

	UINT* InnerData = reinterpret_cast<UINT*>(InData);
	if (InnerData == nullptr)
	{
		return false;
	}

	(*OutBuffer) = new IndexBuffer;
	if (OutBuffer == nullptr)
	{
		return false;
	}

	(*OutBuffer)->Data = new UINT[DataSize];
	if ((*OutBuffer)->Data == nullptr)
	{
		delete OutBuffer;
		OutBuffer = nullptr;
		return false;
	}

	for (UINT i = 0; i < DataSize; ++i)
	{
		(*OutBuffer)->Data[i] = InnerData[i];
	}
	(*OutBuffer)->DataSize = DataSize;

	return true;
}
