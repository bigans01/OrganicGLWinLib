#pragma once

#ifndef PTRIANGLE_H
#define PTRIANGLE_H

#include "RCollisionPoint.h"
#include <glm/glm.hpp>
#include "IndependentUtils.h"
#include "ECBPolyPoint.h"
#include <iostream>
#include "PTrianglePointLinkArray.h"
#include <map>
#include "MassGridArray.h"
#include "RUtils.h"

class PTriangle
{
	public:
		PTriangle() {};
		PTriangle(RCollisionPoint* in_candidatePoint0Ref, 
				  RCollisionPoint* in_candidatePoint1Ref, 
				  RCollisionPoint* in_candidatePoint2Ref, 
				  glm::vec3 in_targetEmptyNormal, 
				  bool in_pTriangleDebugFlag);

		void rebuildEmptyNormal();					// uses cross-product calculation to generate the empty normal 
		void printPTrianglePoints();		
		short getMaterialID();			
		void calculateTriangleMaterial();			// simple function for calculating the triangle's material.
		void sampleTriangleMaterial(MassGridArray* in_massGridArrayRef, 
									float in_tileGridWidth,
									int in_numberOfTilesPerDimension);
		PTrianglePointLinkArray fetchLinkArray();

		glm::vec3 getEmptyNormal();
		RCollisionPoint* collisionPointRefArray[3] = { nullptr };	// an array of references to the RCollisionPoints that make up the PTriangle.
	private:
		bool runInitialEmptyNormalTest(bool in_pTriangleDebugFlag);		// checks whether or not the normalized initial calculation of the empty normal -- which is the cross product
																		// of the points of the PTriangle -- matches the normalized version of the initial value of currentEmptyNormal.
																		// Returns false if the normalized values do not match, which indicates that points 1 and 2 in the PTriangle need to be swapped.
																		// The swapping of the points is handled by the non-default constructor, if the value if false. See the constructor's definition for details.

		glm::vec3 currentEmptyNormal;
		short pTriangleMaterialID = 2;		// needs to be calculated by analyzing the materials in all 3 collision points

};

#endif
