#pragma once

#ifndef PTRIANGLEMESH_H
#define PTRIANGLEMESH_H

#include "PTriangle.h"
#include "MassGridArray.h"
#include "MassZoneBoxType.h"

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

		int determinePTriangleBoundaries(MassZoneBoxType in_boundaryType)
		{
			int totalBoundariesMarked = 0;

			// determine the max boundary used to determine if a triangle is on the POS X/Y/Z border.
			float maxBoundary = 0.0f;
			switch (in_boundaryType)
			{
				case (MassZoneBoxType::BLOCK): { maxBoundary = 1.0f; break;}
				case (MassZoneBoxType::ENCLAVE): { maxBoundary = 4.0f; break;} 
				case (MassZoneBoxType::COLLECTION): { maxBoundary = 4.0f; break; }
			}

			auto trianglesBegin = meshPTriangles.begin();
			auto trianglesEnd = meshPTriangles.end();
			for (; trianglesBegin != trianglesEnd; trianglesBegin++)
			{
				// check for NEG_Z (all points of the PTriangle have a 0.0f for Z)
				if
				(
					(trianglesBegin->second.collisionPointRefArray[0]->currentValue.z == 0.0f)
					&&
					(trianglesBegin->second.collisionPointRefArray[1]->currentValue.z == 0.0f)
					&&
					(trianglesBegin->second.collisionPointRefArray[2]->currentValue.z == 0.0f)
				)
				{
					trianglesBegin->second.pTriangleBoundaryIndicator.setBoundaryIndicator(BoundaryOrientation::NEG_Z);
					totalBoundariesMarked++;
				}

				// check for POS_X (all points of the PTriangle have a maxBoundary for X)
				else if
				(
					(trianglesBegin->second.collisionPointRefArray[0]->currentValue.x == maxBoundary)
					&&
					(trianglesBegin->second.collisionPointRefArray[1]->currentValue.x == maxBoundary)
					&&
					(trianglesBegin->second.collisionPointRefArray[2]->currentValue.x == maxBoundary)
				)
				{
					trianglesBegin->second.pTriangleBoundaryIndicator.setBoundaryIndicator(BoundaryOrientation::POS_X);
					totalBoundariesMarked++;
				}

				// check for POS_Z (all points of the PTriangle have a maxBoundary for Z)
				else if
				(
					(trianglesBegin->second.collisionPointRefArray[0]->currentValue.z == maxBoundary)
					&&
					(trianglesBegin->second.collisionPointRefArray[1]->currentValue.z == maxBoundary)
					&&
					(trianglesBegin->second.collisionPointRefArray[2]->currentValue.z == maxBoundary)
				)
				{
					trianglesBegin->second.pTriangleBoundaryIndicator.setBoundaryIndicator(BoundaryOrientation::POS_Z);
					totalBoundariesMarked++;
				}

				// check for NEG_X (all points of the PTriangle have a 0.0f for X)
				else if
				(
					(trianglesBegin->second.collisionPointRefArray[0]->currentValue.x == 0.0f)
					&&
					(trianglesBegin->second.collisionPointRefArray[1]->currentValue.x == 0.0f)
					&&
					(trianglesBegin->second.collisionPointRefArray[2]->currentValue.x == 0.0f)
				)
				{
					trianglesBegin->second.pTriangleBoundaryIndicator.setBoundaryIndicator(BoundaryOrientation::NEG_X);
					totalBoundariesMarked++;
				}

				// check for POS_Y (all points of the PTriangle have a maxBoundary for Y)
				else if
				(
					(trianglesBegin->second.collisionPointRefArray[0]->currentValue.y == maxBoundary)
					&&
					(trianglesBegin->second.collisionPointRefArray[1]->currentValue.y == maxBoundary)
					&&
					(trianglesBegin->second.collisionPointRefArray[2]->currentValue.y == maxBoundary)
				)
				{
					trianglesBegin->second.pTriangleBoundaryIndicator.setBoundaryIndicator(BoundaryOrientation::POS_Y);
					totalBoundariesMarked++;
				}

				// check for NEG_Y (all points of the PTriangle have 0.0f for Y)
				else if
				(
					(trianglesBegin->second.collisionPointRefArray[0]->currentValue.y == 0.0f)
					&&
					(trianglesBegin->second.collisionPointRefArray[1]->currentValue.y == 0.0f)
					&&
					(trianglesBegin->second.collisionPointRefArray[2]->currentValue.y == 0.0f)
				)
				{
					trianglesBegin->second.pTriangleBoundaryIndicator.setBoundaryIndicator(BoundaryOrientation::NEG_Y);
					totalBoundariesMarked++;
				}
			}

			return totalBoundariesMarked;
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
