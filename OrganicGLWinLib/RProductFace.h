#pragma once

#ifndef RPRODUCTFACE_H
#define RPRODUCTFACE_H

#include "RProductFaceRootPoints.h"
#include "RCollisionPoint.h"
#include "PTriangle.h"
#include "PTriangleMesh.h"

class RProductFace
{
	public:
		RProductFace() {};
		RProductFace(RProductFaceRootPoints in_rootPoints)
		{
			for (int x = 0; x < 4; x++)
			{
				rootPointArray[x] = in_rootPoints.pointArray[x];
			}
		};
		void generateFacePTriangles(glm::vec3 in_targetEmptyNormal, bool in_faceDebugFlag)
		{
			// PTriangle for index 0
			PTriangle pTriangle0(rootPointArray[0], rootPointArray[1], rootPointArray[2], in_targetEmptyNormal, in_faceDebugFlag);
			PTriangle pTriangle1(rootPointArray[2], rootPointArray[3], rootPointArray[0], in_targetEmptyNormal, in_faceDebugFlag);

			facePTriangles[0] = pTriangle0;
			facePTriangles[1] = pTriangle1;
		}

		void copyPTrianglesIntoMesh(PTriangleMesh* in_pTriangleMesh)
		{
			in_pTriangleMesh->insertPTriangle(facePTriangles[0]);
			in_pTriangleMesh->insertPTriangle(facePTriangles[1]);	
		};

		void printPTrianglePoints()
		{
			for (int x = 0; x < 2; x++)
			{
				facePTriangles[x].printPTrianglePoints();
			}
		}

	private:
		RCollisionPoint* rootPointArray[4];
		PTriangle facePTriangles[2];

};

#endif
