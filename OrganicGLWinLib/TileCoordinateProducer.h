#pragma once

#ifndef TILECOORDINATEPRODUCER_H
#define TILECOORDINATEPRODUCER_H

#include <glm/glm.hpp>
#include <iostream>
#include "QuatRotationRecord.h"
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "PointTranslationCheck.h"
#include "ContainerType.h"
#include "OrganicGLWinUtils.h"
#include "TexturePoints.h"

class TileCoordinateProducer
{
public:
	glm::vec3 point0;
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 triangleCentroid;
	glm::vec3 containerCenter;	// the center coordinate of the container in which this triangle belongs
	glm::vec3 triangleNormal;	// the normal we will use to align this triangle to the Z-plane
	glm::vec3 triangleNormalCopy;
	glm::vec3 slidingVector;	// the point value (vector) that will determine how much we have to shift the triangle towards the center point, so that it is on the same plane.
	QuatRotationPoints quatPoints;
	QuatRotationManager rotationManager;
	PointTranslationCheck pointTranslator;
	ContainerType containerType = ContainerType::UNDEFINED;	// always starts as UNDEFINED, but gets set in constructor
	TexturePoints calculatedUV;
	TileCoordinateProducer(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, ContainerType in_containerType);

	TexturePoints getTexturePoints();
private:
	glm::vec3 determineContainerCenter(ContainerType in_containerType);
	glm::vec3 findPlanarSlidingVector(glm::vec3 in_triangleNormal);
	void applyPlanarSlidingVector();
	void alignToZPlane();
	void translateByMaximumDimensionValue();
	void normalizeToTileCoordinates();
	void applyCorrectionsToNormalizedCoordinates();
	void loadUVCoords();
	//TexturePoints getTexturePoints();
};

#endif
