#pragma once

#ifndef TERRAINTRIANGLEPOINT_H
#define TERRAINTRIANGLEPOINT_H


class TerrainTrianglePoint
{
	public:
		TerrainTrianglePoint() {};
		TerrainTrianglePoint(ECBPolyPoint in_threeDLocation,
			ECBPolyPoint in_ttpEmptyNormal,
			float in_uTileCoord,
			float in_vTileCoord,
			int in_atlasTileUCoord,
			int in_atlasTileVCoord
		) :
			threeDLocation(in_threeDLocation),
			ttpEmptyNormal(in_ttpEmptyNormal),
			uTileCoord(in_uTileCoord),
			vTileCoord(in_vTileCoord),
			atlasTileUCoord(in_atlasTileUCoord),
			atlasTileVCoord(in_atlasTileVCoord)
		{};

		ECBPolyPoint threeDLocation;
		ECBPolyPoint ttpEmptyNormal;
		float uTileCoord;
		float vTileCoord;
		int atlasTileUCoord;
		int atlasTileVCoord;

};

#endif
