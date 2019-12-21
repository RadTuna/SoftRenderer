
#include "Precompiled.h"
#include "..\Public\TextureAsset.h"

#include <cassert>
#include <fstream>

#include "Color32.h"

void TextureAsset::Load(const std::string& InPath)
{
	// 해당 파일을 바이너리 리드 모드로 열기
	std::ifstream Fin(InPath, std::ios::in | std::ios::binary);
	assert(Fin.is_open());

	// 타가 파일의 헤더를 읽음.
	TargaHeader TargaFileHeader;
	Fin.read(reinterpret_cast<char*>(&TargaFileHeader), sizeof(TargaHeader));

	// 높이, 너비, bpp를 읽음 // bpp - bit per pixel
	mTextureHeight = static_cast<UINT>(TargaFileHeader.height);
	mTextureWidth = static_cast<UINT>(TargaFileHeader.width);

	// bpp가 32일때만 열기 // RGBA구조의 타가 파일만 허용
	assert(TargaFileHeader.bpp == 32);

	UINT ImageSize = mTextureWidth * mTextureHeight * 4;

	std::unique_ptr<BYTE[]> TargaImage = std::make_unique<BYTE[]>(ImageSize);
	assert(TargaImage != nullptr);

	Fin.read(reinterpret_cast<char*>(TargaImage.get()), sizeof(ImageSize));
	Fin.close();

	mTextureData = std::make_unique<LinearColor[]>(mTextureHeight * mTextureWidth);
	assert(mTextureData != nullptr);

	int Index = 0;
	int K = (mTextureWidth * mTextureHeight * 4) - (mTextureWidth * 4);
	Color32 TempColorBuffer;
	for (UINT j = 0; j < mTextureHeight; ++j)
	{
		for (UINT i = 0; i < mTextureHeight; ++i)
		{
			TempColorBuffer.R = TargaImage[K + 2];  // Red.
			TempColorBuffer.G = TargaImage[K + 1];  // Green.
			TempColorBuffer.B = TargaImage[K + 0];  // Blue
			TempColorBuffer.A = TargaImage[K + 3];  // Alpha

			mTextureData[Index] = LinearColor(TempColorBuffer); // Gamma to Linear

			K += 4;
			++Index;
		}

		K -= (mTextureWidth * 8);
	}
}
