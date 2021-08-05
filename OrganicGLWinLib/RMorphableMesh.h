#pragma once

#ifndef RMORPHABLEMESH_H
#define RMORPHABLEMESH_H

#include <glm/glm.hpp>
#include "RMorphableMeshCubeAreaDefiner.h"
#include "RMorphableMeshCorners.h"
#include "RPointToGridTranslator.h"
#include "Rasterized3DMassGrid.h"
#include "RMorphableMeshCornerArray.h"
#include <map>
#include "ECBPPOrientations.h"
#include "RProductFace.h"
#include "RMorphableMeshState.h"
#include "RCollisionPointModifier.h"

class RMorphableMesh
{
	public:
		RMorphableMesh() {};
		RMorphableMesh(int in_pointsPerDim, 
					   glm::vec3 in_meshCenterCoordinate, 
				       float in_meshDiameter, 
					   RPointToGridTranslator* in_translatorRef, 
					   Rasterized3DMassGrid* in_gridRef) :
			pointsPerDim(in_pointsPerDim),
			meshCenter(in_meshCenterCoordinate),
			meshDiameter(in_meshDiameter),
			translatorRef(in_translatorRef),
			gridRef(in_gridRef)
		{
			morphAreaDefinition.setAreaDimensions(in_meshCenterCoordinate, in_meshDiameter);
		};

		RMorphableMeshCubeAreaDefiner getAreaDefiner();		// Step 1: the calling function needs to get this Mesh's cube area to scan.
		void setMeshCorners(RMorphableMeshCorners in_meshCorners);
		void printCornerPoints();
		void updatePointUsageCounts();
		void runSuctionByXSlice(float in_cubeDimLength, float in_tileDimWeightRatio, int in_tilesPerDim);
		void generateRProductFaces();
		void updatePointState(RMorphableMeshState in_pointState);
		RMorphableMeshState getMeshState();
	private:
		int pointsPerDim = 0;	// if this value is 2, only corner points would exist.
		glm::vec3 meshCenter;
		float meshDiameter = 0.0f;
		RMorphableMeshCubeAreaDefiner morphAreaDefinition;	// called by other functions/classes outside of this one, 
		                                                    // to determine the search area for existing mass
		RMorphableMeshCorners meshCorners;
		RPointToGridTranslator* translatorRef = nullptr;
		Rasterized3DMassGrid* gridRef = nullptr;
		std::map<ECBPPOrientations, RProductFace> rProductFaceMap;
		RMorphableMeshState pointState = RMorphableMeshState::EXPOSED;	// a morphable mesh should always be assumed EXPOSED, until it is determined to be LANDLOCKED.

};

#endif
