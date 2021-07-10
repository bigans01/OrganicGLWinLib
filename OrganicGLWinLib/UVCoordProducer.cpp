#pragma once

#include "stdafx.h"
#include "UVCoordProducer.h"

void UVCoordProducer::setAsActive(EnclaveBlock* in_enclaveBlockPtr, FanBase* in_enclaveBlockTrianglePtr, AtlasMap* in_mapRef, int in_debugFlag)
{

	atlasMapRef = in_mapRef;
	atlasTileID = in_enclaveBlockTrianglePtr->materialID;		// set the materialID
	//std::cout << "atlas tile ID is: " << atlasTileID << std::endl;
	int totalPointsInSecondary = in_enclaveBlockTrianglePtr->numberOfTertiaries + 2;

	// (P4) 
	// OLD CODE
	/*
	for (int x = 0; x < totalPointsInSecondary; x++)
	{
		NibbleMeta current_nibble_target = OrganicUtils::findNibbleByte(currentNibble);		// get the target nibble
		int pointA_index = OrganicUtils::getNibbleFromUnsignedChar(&in_enclaveBlockTrianglePtr->pointArray[current_nibble_target.currentByte], 8 - current_nibble_target.currentNibble);	// get the index of the point to find
		EnclaveBlockVertex vertexToAdd = in_enclaveBlockPtr->getPointAtIndex(pointA_index);;		// acquire the vertex
		ECBPolyPoint convertedPoint = OrganicUtils::convertEnclaveBlockVertexToFloats(vertexToAdd);		// convert it to an ECBPolyPoint
		organicPoly.addPoint(convertedPoint);	// add the converted point to the testPolygon
		if (in_debugFlag == 1)
		{
			//std::cout << "Original vertex point: " << int(vertexToAdd.x) << ", " << int(vertexToAdd.y) << ", " << int(vertexToAdd.z) << std::endl;
			std::cout << "Point added ---->: (index " << pointA_index << ")-> (" << x << "): " << convertedPoint.x << ", " << convertedPoint.y << "," << convertedPoint.z << std::endl;
		}

		currentNibble += 4;
	}
	*/

	for (int x = 0; x < totalPointsInSecondary; x++)
	{
		//NibbleMeta current_nibble_target = OrganicUtils::findNibbleByte(currentNibble);		// get the target nibble
		//int pointA_index = OrganicUtils::getNibbleFromUnsignedChar(&in_enclaveBlockTrianglePtr->pointArray[current_nibble_target.currentByte], 8 - current_nibble_target.currentNibble);	// get the index of the point to find
		//int pointA_index = in_enclaveBlockTrianglePtr->pointArray[x];
		int pointA_index = in_enclaveBlockTrianglePtr->getPointAtIndex(x);
		EnclaveBlockVertex vertexToAdd = in_enclaveBlockPtr->fetchPoint(pointA_index);		// acquire the vertex
		ECBPolyPoint convertedPoint = IndependentUtils::convertEnclaveBlockVertexToFloats(vertexToAdd);		// convert it to an ECBPolyPoint
		organicPoly.addPoint(convertedPoint);	// add the converted point to the testPolygon
		if (in_debugFlag == 1)
		{
			//std::cout << "Original vertex point: " << int(vertexToAdd.x) << ", " << int(vertexToAdd.y) << ", " << int(vertexToAdd.z) << std::endl;
			std::cout << "Point added ---->: (index " << pointA_index << ")-> (" << x << "): " << convertedPoint.x << ", " << convertedPoint.y << "," << convertedPoint.z << std::endl;
		}

		currentNibble += 4;
	}

	// No longer required as of 6/22/2020

	/*
	//organicPoly.setDebugFlag(in_debugFlag);
	//organicPoly.rotateTo2D();
	//organicPoly.findHighestXYDiffs2();


	//organicPoly.findHighestXYDiffs3();
	//organicPoly.checkForDownScale2(in_debugFlag);

	//organicPoly.findHighestXYDiffs4();
	//organicPoly.checkForDownScale3(in_debugFlag);


	//organicPoly.findHighestXYDiffs3();
	//organicPoly.checkForDownScale2(in_debugFlag);
	*/
}

UVTriangleCoords UVCoordProducer::getUVPointsAndIterateToNext()
{
	UVTriangleCoords returnCoords;




	// OPTION 1

	/*
	TexturePoints UV;
	UV.UVpoints[0].U_coord = OrganicUtils::roundToHundredth(organicPoly.getPoint(0).x);
	UV.UVpoints[0].V_coord = OrganicUtils::roundToHundredth(organicPoly.getPoint(0).y);

	UV.UVpoints[1].U_coord = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_second_point_index).x);
	UV.UVpoints[1].V_coord = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_second_point_index).y);

	UV.UVpoints[2].U_coord = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_third_point_index).x);
	UV.UVpoints[2].V_coord = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_third_point_index).y);
	TexturePoints TranslatedPoints = atlasMapRef->getUVPointsForTerrainTile(atlasTileID, UV);
	*/



	glm::vec3 glPoint0, glPoint1, glPoint2;
	glPoint0.x = organicPoly.getPoint(0).x;
	glPoint0.y = organicPoly.getPoint(0).y;
	glPoint0.z = organicPoly.getPoint(0).z;

	glPoint1.x = organicPoly.getPoint(current_second_point_index).x;
	glPoint1.y = organicPoly.getPoint(current_second_point_index).y;
	glPoint1.z = organicPoly.getPoint(current_second_point_index).z;

	glPoint2.x = organicPoly.getPoint(current_third_point_index).x;
	glPoint2.y = organicPoly.getPoint(current_third_point_index).y;
	glPoint2.z = organicPoly.getPoint(current_third_point_index).z;
	TileCoordinateProducer producer(glPoint0, glPoint1, glPoint2, ContainerType::BLOCK, 0, 0, 0);
	TexturePoints UV = producer.getTexturePoints();

	TexturePoints TranslatedPoints = atlasMapRef->getUVPointsForTerrainTile(atlasTileID, UV);


	// get the first point
	returnCoords.UVpoint[0].x = TranslatedPoints.UVpoints[0].U_coord;
	returnCoords.UVpoint[0].y = TranslatedPoints.UVpoints[0].V_coord;

	// get the second point
	returnCoords.UVpoint[1].x = TranslatedPoints.UVpoints[1].U_coord;
	returnCoords.UVpoint[1].y = TranslatedPoints.UVpoints[1].V_coord;

	// get the third point
	returnCoords.UVpoint[2].x = TranslatedPoints.UVpoints[2].U_coord;
	returnCoords.UVpoint[2].y = TranslatedPoints.UVpoints[2].V_coord;

	// get the tile location
	returnCoords.U_tile_coord = TranslatedPoints.U_tile_coord;
	returnCoords.V_tile_coord = TranslatedPoints.V_tile_coord;


	// OPTION 2
	/*
	returnCoords.UVpoint[0].x = OrganicUtils::roundToHundredth(organicPoly.getPoint(0).x);
	returnCoords.UVpoint[0].y = OrganicUtils::roundToHundredth(organicPoly.getPoint(0).y);

	// get the second point		OrganicUtils::roundToHundredth(organicPoly.getPoint(current_second_point_index).x);
	returnCoords.UVpoint[1].x = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_second_point_index).x);
	returnCoords.UVpoint[1].y = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_second_point_index).y);

	// get the third point		OrganicUtils::roundToHundredth(organicPoly.getPoint(current_third_point_index).y);
	returnCoords.UVpoint[2].x = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_third_point_index).x);
	returnCoords.UVpoint[2].y = OrganicUtils::roundToHundredth(organicPoly.getPoint(current_third_point_index).y);
	*/





	// OPTION 3
	/*
	TexturePoints UV;
	UV.UVpoints[0].U_coord = organicPoly.getPoint(0).x;
	UV.UVpoints[0].V_coord = organicPoly.getPoint(0).y;

	UV.UVpoints[1].U_coord = organicPoly.getPoint(current_second_point_index).x;
	UV.UVpoints[1].V_coord = organicPoly.getPoint(current_second_point_index).y;

	UV.UVpoints[2].U_coord = organicPoly.getPoint(current_third_point_index).x;
	UV.UVpoints[2].V_coord = organicPoly.getPoint(current_third_point_index).y;
	TexturePoints TranslatedPoints = atlasMapRef->getUVPointsForTerrainTile(2, UV);


	// get the first point
	returnCoords.UVpoint[0].x = TranslatedPoints.UVpoints[0].U_coord;
	returnCoords.UVpoint[0].y = TranslatedPoints.UVpoints[0].V_coord;

	// get the second point
	returnCoords.UVpoint[1].x = TranslatedPoints.UVpoints[1].U_coord;
	returnCoords.UVpoint[1].y = TranslatedPoints.UVpoints[1].V_coord;

	// get the third point
	returnCoords.UVpoint[2].x = TranslatedPoints.UVpoints[2].U_coord;
	returnCoords.UVpoint[2].y = TranslatedPoints.UVpoints[2].V_coord;
	*/


	current_second_point_index++;
	current_third_point_index++;

	return returnCoords;

}