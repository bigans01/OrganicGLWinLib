#pragma once

#ifndef RMORPHABLEMESH_H
#define RMORPHABLEMESH_H

#include <glm/glm.hpp>
#include "RMorphableMeshCubeAreaDefiner.h"
#include "RMorphableMeshCorners.h"

class RMorphableMesh
{
	public:
		RMorphableMesh() {};
		RMorphableMesh(int in_pointsPerDim, glm::vec3 in_meshCenterCoordinate, float in_meshDiameter) :
			pointsPerDim(in_pointsPerDim),
			meshCenter(in_meshCenterCoordinate),
			meshDiameter(in_meshDiameter)
		{
			morphAreaDefinition.setAreaDimensions(in_meshCenterCoordinate, in_meshDiameter);
		};

		RMorphableMeshCubeAreaDefiner getAreaDefiner();		// Step 1: the calling function needs to get this Mesh's cube area to scan.
		void setMeshCorners(RMorphableMeshCorners in_meshCorners);
		void printCornerPoints();
		void updatePointUsageCounts();
	private:
		int pointsPerDim = 0;	// if this value is 2, only corner points would exist.
		glm::vec3 meshCenter;
		float meshDiameter = 0.0f;
		RMorphableMeshCubeAreaDefiner morphAreaDefinition;	// called by other functions/classes outside of this one, 
		                                                    // to determine the search area for existing mass
		RMorphableMeshCorners meshCorners;

};

#endif
