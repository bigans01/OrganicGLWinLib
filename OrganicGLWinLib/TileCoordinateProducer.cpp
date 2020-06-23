#include "stdafx.h"
#include "TileCoordinateProducer.h"

TileCoordinateProducer::TileCoordinateProducer(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, ContainerType in_containerType)
{
	// load point values
	point0 = in_point0;
	point1 = in_point1;
	point2 = in_point2;

	containerType = in_containerType;

	// find the triangle centroid
	triangleCentroid = OrganicGLWinUtils::findTriangleCentroid(point0, point1, point2);

	// find the triangle normal
	triangleNormal = OrganicGLWinUtils::findTriangleNormal(point0, point1, point2);

	// use the container type value to determine the container's center point to get
	containerCenter = determineContainerCenter(in_containerType);

	// create copies of the calculated values;
	glm::vec3 point0Copy = point0;
	glm::vec3 point1Copy = point1;
	glm::vec3 point2Copy = point2;
	glm::vec3 triangleCentroidCopy = triangleCentroid;
	triangleNormalCopy = triangleNormal;
	glm::vec3 containerCenterCopy = containerCenter;

	//std::cout << "!!! centroid, prior to translation: " << triangleCentroidCopy.x << ", " << triangleCentroidCopy.y << ", " << triangleCentroidCopy.z << std::endl;
	//std::cout << "!!! container center, prior to translation: " << containerCenterCopy.x << ", " << containerCenterCopy.y << ", " << containerCenterCopy.z << std::endl;

	// push back the copied values into the quat points
	quatPoints.pointsRef.push_back(&point0Copy);
	quatPoints.pointsRef.push_back(&point1Copy);
	quatPoints.pointsRef.push_back(&point2Copy);
	quatPoints.pointsRef.push_back(&triangleCentroidCopy);
	quatPoints.pointsRef.push_back(&containerCenterCopy);

	// find the planar sliding vector
	findPlanarSlidingVector(triangleNormalCopy);

	// clear out the quat points; load them with the original points of the triangle
	quatPoints.clearPoints();
	quatPoints.pointsRef.push_back(&point0);
	quatPoints.pointsRef.push_back(&point1);
	quatPoints.pointsRef.push_back(&point2);

	// apply the planar sliding vector
	applyPlanarSlidingVector();

	// add the container's center as the next point
	containerCenterCopy = containerCenter;
	quatPoints.pointsRef.push_back(&containerCenterCopy);

	// add the normal to the quat points, after copying the original value again
	//triangleNormalCopy = triangleNormal;
	//quatPoints.pointsRef.push_back(&triangleNormalCopy);

	// move the plane that slid so that Z is 0.
	alignToZPlane();

	// translate according to the container type's maximum dimension values
	translateByMaximumDimensionValue();

	// normalize the points to tile coordinates
	normalizeToTileCoordinates();

	// apply dimension corrections (if below 0, go to 0...if above 1, go to 1)
	applyCorrectionsToNormalizedCoordinates();

	// store the UV coords
	loadUVCoords();

}

glm::vec3 TileCoordinateProducer::determineContainerCenter(ContainerType in_containerType)
{
	glm::vec3 returnVec;
	if (in_containerType == ContainerType::BLOCK)
	{
		returnVec.x = .5f;
		returnVec.y = .5f;
		returnVec.z = .5f;
	}
	else if (in_containerType == ContainerType::ENCLAVE)
	{
		returnVec.x = 2.0f;
		returnVec.y = 2.0f;
		returnVec.z = 2.0f;
	}
	else if (in_containerType == ContainerType::COLLECTION)
	{
		returnVec.x = 16.0f;
		returnVec.y = 16.0f;
		returnVec.z = 16.0f;
	}
	return returnVec;
}

glm::vec3 TileCoordinateProducer::findPlanarSlidingVector(glm::vec3 in_triangleNormal)
{
	glm::vec3 returnVec;

	// check for translation, by analyzing the triangle centroid.
	pointTranslator.performCheck(quatPoints.getPointByIndex(3));
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// push back the normal into the quat Points, only after translation has ocurred (normals shouldn't be translated)
	// order of points after this should be:
	// [0] point 0 of triangle
	// [1] point 1 of triangle
	// [2] point 2 of triangle
	// [3] centroid of the triangle
	// [4] center point of the container that the triangle is in
	// [5] normal of the triangle (not translated)
	// [6] the resulting sliding vector that is calculated later
	quatPoints.pointsRef.push_back(&triangleNormalCopy);
	quatPoints.pointsRef.push_back(&slidingVector);

	glm::vec3 centroidPrint = quatPoints.getPointByIndex(3);
	//std::cout << "#### Centroid point after translation is now at: " << centroidPrint.x << ", " << centroidPrint.y << ", " << centroidPrint.z << std::endl;

	glm::vec3 containerCenterPrint = quatPoints.getPointByIndex(4);
	//std::cout << "#### Container center point after translation is now at: " << containerCenterPrint.x << ", " << containerCenterPrint.y << ", " << containerCenterPrint.z << std::endl;

	slidingVector = rotationManager.initializeAndRunForPlanarSlide(&quatPoints); // find the sliding vector.



	// go back to the original points
	/*
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getReverseTranslationValue());
	}
	*/


	//rotationManager.executeRotationsForPlanarSlide();

	return slidingVector;
}

void TileCoordinateProducer::applyPlanarSlidingVector()
{
	quatPoints.applyTranslation(slidingVector);
}

void TileCoordinateProducer::alignToZPlane()
{
	//std::cout << "##### Attempting Z-plane alignment " << std::endl;
	// check for translation, by analyzing the container centroid. (should be index 3 at this point)
	pointTranslator.performCheck(quatPoints.getPointByIndex(3));
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// push back the normal into the quat Points, only after translation has ocurred (normals shouldn't be translated)
	// order of points after this should be:
	// [0] point 0 of triangle
	// [1] point 1 of triangle
	// [2] point 2 of triangle
	// [3] center of the container
	// [4] normal of the triangle (not translated)
	quatPoints.pointsRef.push_back(&triangleNormalCopy);
	rotationManager.initializeAndRunForPlanarAlignmentToZ(&quatPoints);
}

void TileCoordinateProducer::translateByMaximumDimensionValue()
{
	glm::vec3 translationValue;
	if (containerType == ContainerType::BLOCK)
	{
		translationValue.x = 1.41421f / 2;
		translationValue.y = 1.41421f / 2;
		translationValue.z = 1.41421f / 2;
	}
	else if (containerType == ContainerType::ENCLAVE)
	{
		// calculate max dimension value for an Enclave and put it here later
	}
	else if (containerType == ContainerType::COLLECTION)
	{
		// calculate max dimension value for a Collection and put it here later
	}

	quatPoints.applyTranslation(translationValue);

	//std::cout << "######### quat points after applying maximum dimension value ############### " << std::endl;
	//quatPoints.printPoints();
}

void TileCoordinateProducer::normalizeToTileCoordinates()
{
	float normalizationValue;
	if (containerType == ContainerType::BLOCK)
	{
		normalizationValue = 1.41421f;
	}
	else if (containerType == ContainerType::ENCLAVE)
	{
		// calculate max dimension value for an Enclave and put it here later
	}
	else if (containerType == ContainerType::COLLECTION)
	{
		// calculate max dimension value for a Collection and put it here later
	}
	quatPoints.applyNormalization(normalizationValue);

	//std::cout << "######### quat points after applying normalization value ############### " << std::endl;
	//quatPoints.printPoints();
}

void TileCoordinateProducer::applyCorrectionsToNormalizedCoordinates()
{
	quatPoints.applyDimensionCorrections();
	//std::cout << "######### quat points after applying dimension correction ############### " << std::endl;
	//quatPoints.printPoints();
}

void TileCoordinateProducer::loadUVCoords()
{
	calculatedUV.UVpoints[0].U_coord = point0.x;
	calculatedUV.UVpoints[0].V_coord = point0.y;

	calculatedUV.UVpoints[1].U_coord = point1.x;
	calculatedUV.UVpoints[1].V_coord = point1.y;

	calculatedUV.UVpoints[2].U_coord = point2.x;
	calculatedUV.UVpoints[2].V_coord = point2.y;

	//std::cout << "********* UV coords are: *************" << std::endl;
	//std::cout << "[0] " << calculatedUV.UVpoints[0].U_coord << ", " << calculatedUV.UVpoints[0].V_coord << std::endl;
	//std::cout << "[1] " << calculatedUV.UVpoints[1].U_coord << ", " << calculatedUV.UVpoints[1].V_coord << std::endl;
	//std::cout << "[2] " << calculatedUV.UVpoints[2].U_coord << ", " << calculatedUV.UVpoints[2].V_coord << std::endl;
}

TexturePoints TileCoordinateProducer::getTexturePoints()
{
	return calculatedUV;
}