#include "stdafx.h"
#include "UVCoordProducerECBPoly.h"

UVCoordProducerECBPoly::UVCoordProducerECBPoly(TriangleMaterial in_materialID, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2, AtlasMap* in_mapRef, int in_debugFlag, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	point0 = in_point0;
	point1 = in_point1;
	point2 = in_point2;
	atlasMapRef = in_mapRef;
	atlasTileID = in_materialID;		// set the materialID
	blueprintKey = in_blueprintKey;
}

UVTriangleCoords UVCoordProducerECBPoly::getCoords()
{
	UVTriangleCoords returnCoords;

	glm::vec3 glPoint0, glPoint1, glPoint2;
	glPoint0.x = point0.x;
	glPoint0.y = point0.y;
	glPoint0.z = point0.z;

	glPoint1.x = point1.x;
	glPoint1.y = point1.y;
	glPoint1.z = point1.z;

	glPoint2.x = point2.x;
	glPoint2.y = point2.y;
	glPoint2.z = point2.z;

	TileCoordinateProducer producer(glPoint0, glPoint1, glPoint2, ContainerType::COLLECTION, blueprintKey.x, blueprintKey.y, blueprintKey.z);
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
	return returnCoords;
}