#include "stdafx.h"
#include "TileCoordinateProducer.h"

TileCoordinateProducer::TileCoordinateProducer(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, ContainerType in_containerType, int in_blueprintKeyX, int in_blueprintKeyY, int in_blueprintKeyZ)
{
	// load point values
	point0 = in_point0;
	point1 = in_point1;
	point2 = in_point2;

	blueprintKey_x = in_blueprintKeyX;
	blueprintKey_y = in_blueprintKeyY;
	blueprintKey_z = in_blueprintKeyZ;

	containerType = in_containerType;

	// find the triangle centroid
	triangleCentroid = OrganicGLWinUtils::findTriangleCentroid(point0, point1, point2);

	// find the triangle normal
	triangleNormal = OrganicGLWinUtils::findTriangleNormal(point0, point1, point2);

	// use the container type value to determine the container's center point to get
	containerCenter = determineContainerCenter(in_containerType);

	// create copies of the calculated values;

	/*
	if (containerType == ContainerType::COLLECTION)
	{
		//std::cout << "********* Original points, before translation/modification: " << std::endl;
		//std::cout << "[0]: " << point0.x << ", " << point0.y << ", " << point0.z << std::endl;
		//std::cout << "[1]: " << point1.x << ", " << point1.y << ", " << point1.z << std::endl;
		//std::cout << "[2]: " << point2.x << ", " << point2.y << ", " << point2.z << std::endl;

		//std::cout << "********* Blueprint key is: " << std::endl;
		//std::cout << blueprintKey_x << ", " << blueprintKey_y << ", " << blueprintKey_z << std::endl;

		//OrganicGLWinUtils::findTriangleCentroidDebug(point0, point1, point2);
	}
	*/

	glm::vec3 point0Copy = point0;
	glm::vec3 point1Copy = point1;
	glm::vec3 point2Copy = point2;
	glm::vec3 triangleCentroidCopy = triangleCentroid;
	triangleNormalCopy = triangleNormal;
	glm::vec3 containerCenterCopy = containerCenter;

	//std::cout << "!!! centroid, prior to translation: " << triangleCentroidCopy.x << ", " << triangleCentroidCopy.y << ", " << triangleCentroidCopy.z << std::endl;
	//std::cout << "!!! container center, prior to translation: " << containerCenterCopy.x << ", " << containerCenterCopy.y << ", " << containerCenterCopy.z << std::endl;

	if (containerType == ContainerType::COLLECTION)
	{
		//std::cout << "!! Planar sliding vector will be: " << tempVec.x << ", " << tempVec.y << ", " << tempVec.z << std::endl;
		//std::cout << "!! (1) Centroid is: " << triangleCentroidCopy.x << ", " << triangleCentroidCopy.y << ", " << triangleCentroidCopy.z << std::endl;
	}


	// push back the copied values into the quat points
	quatPoints.pointsRefVector.push_back(&point0Copy);
	quatPoints.pointsRefVector.push_back(&point1Copy);
	quatPoints.pointsRefVector.push_back(&point2Copy);
	quatPoints.pointsRefVector.push_back(&triangleCentroidCopy);
	quatPoints.pointsRefVector.push_back(&containerCenterCopy);

	// find the planar sliding vector
	glm::vec3 tempVec = findPlanarSlidingVector(triangleNormalCopy, blueprintKey_x, blueprintKey_y, blueprintKey_z);
	if (containerType == ContainerType::COLLECTION)
	{
		//std::cout << "!! Planar sliding vector will be: " << tempVec.x << ", " << tempVec.y << ", " << tempVec.z << std::endl;
		//std::cout << "!! (2) Centroid is: " << triangleCentroidCopy.x << ", " << triangleCentroidCopy.y << ", " << triangleCentroidCopy.z << std::endl;
	}

	// clear out the quat points; load them with the original points of the triangle
	quatPoints.clearPoints();
	quatPoints.pointsRefVector.push_back(&point0);
	quatPoints.pointsRefVector.push_back(&point1);
	quatPoints.pointsRefVector.push_back(&point2);

	// apply the planar sliding vector
	applyPlanarSlidingVector();

	// add the container's center as the next point
	containerCenterCopy = containerCenter;
	quatPoints.pointsRefVector.push_back(&containerCenterCopy);

	// add the normal to the quat points, after copying the original value again
	//triangleNormalCopy = triangleNormal;
	//quatPoints.pointsRefVector.push_back(&triangleNormalCopy);

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

glm::vec3 TileCoordinateProducer::findPlanarSlidingVector(glm::vec3 in_triangleNormal, int in_blueprintKeyX, int in_blueprintKeyY, int in_blueprintKeyZ)
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
	quatPoints.pointsRefVector.push_back(&triangleNormalCopy);
	quatPoints.pointsRefVector.push_back(&slidingVector);

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
	pointTranslator.performCheck(quatPoints.getPointByIndex(3), blueprintKey_x, blueprintKey_y, blueprintKey_z);
	if (pointTranslator.requiresTranslation == 1)
	{
		/*
		if (containerType == ContainerType::COLLECTION)
		{
			//std::cout << "!!! Translation required " << std::endl;
			//std::cout << "--points are sliding vector is applied are: " << std::endl;
			//std::cout << "[0]: " << point0.x << ", " << point0.y << ", " << point0.z << std::endl;
			//std::cout << "[1]: " << point1.x << ", " << point1.y << ", " << point1.z << std::endl;
			//std::cout << "[2]: " << point2.x << ", " << point2.y << ", " << point2.z << std::endl;

			//quatPoints.printTrianglePoints();
		}
		*/
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
		/*
		if (containerType == ContainerType::COLLECTION)
		{
			//quatPoints.printTrianglePoints();
			glm::vec3 returnVec;
			returnVec = pointTranslator.getTranslationValue();

			//std::cout << "Sliding vector value is: " << returnVec.x << ", " << returnVec.y << ", " << returnVec.z << std::endl;

			//quatPoints.printPoints();
		}
		*/
	}

	// push back the normal into the quat Points, only after translation has ocurred (normals shouldn't be translated)
	// order of points after this should be:
	// [0] point 0 of triangle
	// [1] point 1 of triangle
	// [2] point 2 of triangle
	// [3] center of the container
	// [4] normal of the triangle (not translated)
	quatPoints.pointsRefVector.push_back(&triangleNormalCopy);
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
		translationValue.x = ((1.41421f) * 32) / 2;
		translationValue.y = ((1.41421f) * 32) / 2;
		translationValue.z = ((1.41421f) * 32) / 2;
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
		normalizationValue = 1.41421f * 32;
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

	if (containerType == ContainerType::COLLECTION)
	{
		//std::cout << "********* UV coords are: *************" << std::endl;
		//std::cout << "[0] " << calculatedUV.UVpoints[0].U_coord << ", " << calculatedUV.UVpoints[0].V_coord << std::endl;
		//std::cout << "[1] " << calculatedUV.UVpoints[1].U_coord << ", " << calculatedUV.UVpoints[1].V_coord << std::endl;
		//std::cout << "[2] " << calculatedUV.UVpoints[2].U_coord << ", " << calculatedUV.UVpoints[2].V_coord << std::endl;
	}
}

TexturePoints TileCoordinateProducer::getTexturePoints()
{
	return calculatedUV;
}