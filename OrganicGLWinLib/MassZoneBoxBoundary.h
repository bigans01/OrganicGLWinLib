#pragma once

#ifndef MASSZONEBOXBOUNDARY_H
#define MASSZONEBOXBOUNDARY_H

#include <map>
#include "MassZoneBoxBoundaryState.h"
#include <glm/glm.hpp>
#include <iostream>
#include "SPoly.h"
#include "STriangle.h"

class MassZoneBoxBoundary
{
	public:
		MassZoneBoxBoundary() {};
		MassZoneBoxBoundary(glm::vec3 in_corner1, glm::vec3 in_corner2, glm::vec3 in_corner3, glm::vec3 in_corner4, glm::vec3 in_emptyNormal)
		{
			insertCornerPoint(in_corner1);
			insertCornerPoint(in_corner2);
			insertCornerPoint(in_corner3);
			insertCornerPoint(in_corner4);
			emptyNormal = in_emptyNormal;
			buildBoundarySPoly();
		};
		MassZoneBoxBoundaryState boundaryCurrentState = MassZoneBoxBoundaryState::INACTIVE;	// the boundary always starts as inactive.
		std::map<int, glm::vec3> cornerPointMap;

		void printPoints()
		{
			auto cornerPointMapBegin = cornerPointMap.begin();
			auto cornerPointMapEnd = cornerPointMap.end();
			for (; cornerPointMapBegin != cornerPointMapEnd; cornerPointMapBegin++)
			{
				std::cout << "[" << cornerPointMapBegin->first << "]: " << cornerPointMapBegin->second.x << ", "
																<< cornerPointMapBegin->second.y << ", "
																<< cornerPointMapBegin->second.z << std::endl;
			}

			std::cout << "Empty Normal: " << emptyNormal.x << ", " << emptyNormal.y << ", " << emptyNormal.z << std::endl;
		}

	
	private:
		void insertCornerPoint(glm::vec3 in_cornerPoint)
		{
			int cornerPointMapIndex = cornerPointMap.size();
			cornerPointMap[cornerPointMapIndex] = in_cornerPoint;
		}
		void buildBoundarySPoly()
		{
			STriangle triangleA(cornerPointMap[0], cornerPointMap[1], cornerPointMap[2]);
			STriangle triangleB(cornerPointMap[0], cornerPointMap[2], cornerPointMap[3]);
			boundarySPoly.addTriangle(triangleA);
			boundarySPoly.addTriangle(triangleB);
			boundarySPoly.massManipulationSetting = MassManipulationMode::DESTRUCTION;	// all boundary SPolys should be set to DESTRUCTION, because 
																					    // we want to produce a new SPoly that is opposite the normals of the 
																						// CategorizedLines that are within it. It's not really "DESTRUCTION",
																						// but the setting is appropriate to use here.

			// while normally called by SPolySet::configurePolysWithoutNormalCalcs(), these need to be called separately,
			// since nothing else will do this.
			boundarySPoly.determinePrimalPoints();
			boundarySPoly.determineBorderLines();
		}
		SPoly boundarySPoly;
		glm::vec3 emptyNormal;
};

#endif
