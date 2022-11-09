#pragma once

#ifndef UVCOORDPRODUCERENCLAVETRIANGLE_H
#define UVCOORDPRODUCERENCLAVETRIANGLE_H

#include "UVTriangleCoords.h"
#include "TileCoordinateProducer.h"
#include "AtlasMap.h"

class UVCoordProducerEnclaveTriangle
{
public:
	UVCoordProducerEnclaveTriangle(int in_materialID, ECBPolyPoint in_point0, ECBPolyPoint in_point1, ECBPolyPoint in_point2, AtlasMap* in_mapRef, int in_debugFlag, EnclaveKeyDef::EnclaveKey in_blueprintKey);
	UVTriangleCoords getCoords();
private:
	ECBPolyPoint point0;
	ECBPolyPoint point1;
	ECBPolyPoint point2;
	AtlasMap* atlasMapRef;
	short atlasTileID = 0;		// the ID of the atlas tile to look up -- equivalent to polyMaterialID
	EnclaveKeyDef::EnclaveKey blueprintKey;
	int debugflag = 0;
};

#endif