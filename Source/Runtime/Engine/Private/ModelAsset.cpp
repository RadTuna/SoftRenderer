
#include "Precompiled.h"
#include "..\Public\ModelAsset.h"

#include <cassert>
#include <fstream>

#include "EngineUtility.h"

void ModelAsset::Load(const std::string& InPath)
{
	std::vector<Vector3> VertPositions;
	std::vector<UINT> IndexPositions;

	std::vector<Vector2> VertUVs;
	std::vector<UINT> IndexUVs;

	std::vector<Vector3> VertNormals;
	std::vector<UINT> IndexNormals;

	std::ifstream Fin(InPath, std::ios::in);
	assert(Fin.is_open());

	while (true)
	{
		std::string CurLine;
		Fin >> CurLine;

		if (Fin.eof() || Fin.bad() || Fin.fail())
		{
			break;
		}
		else if (CurLine == "v")
		{
			Vector3 CurPos;
			Fin >> CurPos.X >> CurPos.Y >> CurPos.Z;
			VertPositions.emplace_back(CurPos);
		}
		else if (CurLine == "vt")
		{
			Vector2 CurUV;
			Fin >> CurUV.X >> CurUV.Y;
			VertUVs.emplace_back(CurUV);
		}
		else if (CurLine == "vn")
		{
			Vector3 CurNormal;
			Fin >> CurNormal.X >> CurNormal.Y >> CurNormal.Z;
			VertNormals.emplace_back(CurNormal);
		}
		else if (CurLine == "f")
		{
			std::string CurDataString;
			std::vector<UINT> CurDataSet;
			CurDataSet.reserve(3);

			for (int i = 0; i < 3; ++i)
			{
				CurDataSet.clear();

				Fin >> CurDataString;
				CurDataSet = EngineUtility::SplitString<UINT>(CurDataString, '/');
				assert(CurDataSet.size() == 3);

				IndexPositions.emplace_back(CurDataSet[0]);
				IndexUVs.emplace_back(CurDataSet[1]);
				IndexNormals.emplace_back(CurDataSet[2]);
			}
		}
	}

	assert(IndexPositions.size() == IndexUVs.size() && IndexUVs.size() == IndexNormals.size());

	mVerticesLength = mIndicesLength = IndexPositions.size();

	mVertices = std::make_unique<VertexDataType[]>(mVerticesLength);
	mIndices = std::make_unique<UINT[]>(mIndicesLength);
	for (int i = 0; i < IndexPositions.size(); ++i)
	{
		mVertices[i].Position = VertPositions[IndexPositions[i] - 1];
		mVertices[i].UV = VertUVs[IndexUVs[i] - 1];
		mVertices[i].Normal = VertNormals[IndexNormals[i] - 1];
		mIndices[i] = i;
	}
}
