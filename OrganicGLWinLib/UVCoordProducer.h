#pragma once

#ifndef UVCOORDPRODUCER_H
#define UVCOORDPRODUCER_H

#include "stdafx.h"
#include "EnclaveBlock.h"
#include "EnclaveBlockVertex.h"
#include "AtlasMap.h"
#include "IndependentUtils.h"
#include "OrganicPolygon.h"
#include "UVTriangleCoords.h"
#include "TexturePoints.h"
#include "TileCoordinateProducer.h"
#include "ContainerType.h"
#include "FanBase.h"


class UVCoordProducer
{
public:
	void setAsActive(EnclaveBlock* in_enclaveBlockPtr, FanBase* in_enclaveBlockTrianglePtr, AtlasMap* in_mapRef, int in_debugFlag);
	UVTriangleCoords getUVPointsAndIterateToNext();
private:
	OrganicPolygon organicPoly;
	int currentNibble = 0;	// nibble index
	int current_second_point_index = 1;		// always starts at 1
	int current_third_point_index = 2;		// always starts at 2
	short atlasTileID = 0;		// the ID of the atlas tile to look up -- equivalent to polyMaterialID
	AtlasMap* atlasMapRef;
};

#endif
