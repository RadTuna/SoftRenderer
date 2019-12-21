#pragma once

#include <memory>

#include "Asset.h"
#include "LinearColor.h"

struct TextureDataType
{
	LinearColor* ColorData;
	UINT DataSize;
	UINT TexHeight;
	UINT TexWidth;
};

struct TextureBufferDataType
{
	TextureDataType AlbedoMap;
};

class TextureAsset final : public Asset
{
private:

	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:

	explicit TextureAsset() noexcept = default;
	virtual ~TextureAsset() noexcept = default;

	// Load Targa file
	void Load(const std::string& InPath) override;

private:

	std::unique_ptr<LinearColor[]> mTextureData;
	UINT mDataSize;
	UINT mTextureHeight;
	UINT mTextureWidth;

public:

	FORCEINLINE LinearColor* GetTextureData() const { return mTextureData.get(); }
	FORCEINLINE UINT GetTextureDataSize() const { return mDataSize; }
	FORCEINLINE UINT GetTextureHeight() const { return mTextureHeight; }
	FORCEINLINE UINT GetTextureWidth() const { return mTextureWidth; }

};
