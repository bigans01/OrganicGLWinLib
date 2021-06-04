#pragma once

#ifndef RTRIANGLELINE_H
#define RTRIANGLELINE_H

#include <unordered_set>
#include "EnclaveKeyDef.h"
#include <glm/glm.hpp>
#include "STriangleLine.h"

class RTriangleLine
{
	public:
		RTriangleLine() {};
		RTriangleLine(glm::vec3 in_rasterGridPointA, 
					  glm::vec3 in_rasterGridPointB, 
					  EnclaveKeyDef::EnclaveKey in_pointACubeKey,
					  EnclaveKeyDef::EnclaveKey in_pointBCubeKey,
					  float in_rPolyCubeDimLength,
					  STriangleLine in_sTriangleLineToBuildFrom) :
			rLinePointA(in_rasterGridPointA),
			rLinePointB(in_rasterGridPointB),
			rPolyCubeDimLength(in_rPolyCubeDimLength),
			pointACubeKey(in_pointACubeKey),
			pointBCubeKey(in_pointBCubeKey)
		{
			rLineIsBorderLine = in_sTriangleLineToBuildFrom.isBorderLine;
			rLineBorderLineID = in_sTriangleLineToBuildFrom.borderLineID;
			isPointAOnBorderLine = in_sTriangleLineToBuildFrom.pointAOnBorderLine;
			isPointBOnBorderLine = in_sTriangleLineToBuildFrom.pointBOnBorderLine;
		};

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> findBlocksAtX(int in_x)
		{
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> x_set;
			auto rasterizedBlocksBegin = rasterizedBlocks.begin();
			auto rasterizedBlocksEnd = rasterizedBlocks.end();
			for (; rasterizedBlocksBegin != rasterizedBlocksEnd; rasterizedBlocksBegin++)
			{
				if (rasterizedBlocksBegin->x == in_x)
				{
					x_set.insert(*rasterizedBlocksBegin);
				}
			}
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> findBlocksAtY(int in_y)
		{
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> y_set;
			auto rasterizedBlocksBegin = rasterizedBlocks.begin();
			auto rasterizedBlocksEnd = rasterizedBlocks.end();
			for (; rasterizedBlocksBegin != rasterizedBlocksEnd; rasterizedBlocksBegin++)
			{
				if (rasterizedBlocksBegin->y == in_y)
				{
					y_set.insert(*rasterizedBlocksBegin);
				}
			}
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> findBlocksAtZ(int in_z)
		{
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> z_set;
			auto rasterizedBlocksBegin = rasterizedBlocks.begin();
			auto rasterizedBlocksEnd = rasterizedBlocks.end();
			for (; rasterizedBlocksBegin != rasterizedBlocksEnd; rasterizedBlocksBegin++)
			{
				if (rasterizedBlocksBegin->z == in_z)
				{
					z_set.insert(*rasterizedBlocksBegin);
				}
			}
		}

		glm::vec3 rLinePointA;	// the beginning of the line
		glm::vec3 rLinePointB;	// the end of the line
		EnclaveKeyDef::EnclaveKey pointACubeKey;	// the cubes that the points reside in
		EnclaveKeyDef::EnclaveKey pointBCubeKey;	// ""
		float rPolyCubeDimLength = 0.0f;			// the dimension length for xyz, for a rasterization cube.
		unsigned char rLineIsBorderLine = 0;		// indicates whether or not it is a border line of the STriangleSet this triangle belongs in
		unsigned char rLineBorderLineID = 0;		// indicates the borderLineID, if it is indeed a border line
		unsigned char isPointAOnBorderLine = 0;	// is point A on border line? (checked when borderLineID is 1)
		unsigned char isPointBOnBorderLine = 0;	// is point B on border line? " " " 
	private:
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> rasterizedBlocks;

};

#endif
