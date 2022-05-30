#pragma once

#ifndef PTRIANGLEMESH_H
#define PTRIANGLEMESH_H

#include "PTriangle.h"
#include <map>
#include "MassGridArray.h"

class PTriangleMesh
{
	public:
		PTriangleMesh& operator+=(const PTriangleMesh& in_otherPTriangleMesh)
		{
			// will append all ptriangles from the other mesh into this one
			auto otherMeshTrianglesBegin = in_otherPTriangleMesh.meshPTriangles.begin();
			auto otherMeshTrianglesEnd = in_otherPTriangleMesh.meshPTriangles.end();
			for (; otherMeshTrianglesBegin != otherMeshTrianglesEnd; otherMeshTrianglesBegin++)
			{
				insertPTriangle(otherMeshTrianglesBegin->second);
			}
		};

		void insertPTriangle(PTriangle in_pTriangle)
		{
			meshPTriangles[currentPTriangleIndex++] = in_pTriangle;
		}

		int getMeshPTriangleCount()
		{
			return int(meshPTriangles.size());
		}

		void rebuildPTriangleNormals()
		{
			auto trianglesBegin = meshPTriangles.begin();
			auto trianglesEnd = meshPTriangles.end();
			for (; trianglesBegin != trianglesEnd; trianglesBegin++)
			{
				trianglesBegin->second.rebuildEmptyNormal();
			}
		}

		void calculatePTriangleMaterials()
		{
			auto trianglesBegin = meshPTriangles.begin();
			auto trianglesEnd = meshPTriangles.end();
			for (; trianglesBegin != trianglesEnd; trianglesBegin++)
			{
				trianglesBegin->second.calculateTriangleMaterial();
			}
		}

		void samplePTriangleMaterials(MassGridArray* in_massGridArrayRef,
			float in_tileGridWidth,
			int in_numberOfTilesPerDimension)
		{
			auto trianglesBegin = meshPTriangles.begin();
			auto trianglesEnd = meshPTriangles.end();
			for (; trianglesBegin != trianglesEnd; trianglesBegin++)
			{
				trianglesBegin->second.sampleTriangleMaterial(in_massGridArrayRef, in_tileGridWidth, in_numberOfTilesPerDimension);
			}
		}

		std::map<int, PTriangle> meshPTriangles;
	private:
		int currentPTriangleIndex = 0;


};

#endif
