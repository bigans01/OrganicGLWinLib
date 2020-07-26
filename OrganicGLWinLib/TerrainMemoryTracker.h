#pragma once

#ifndef TERRAINMEMORYTRACKER_H
#define TERRAINMEMORYTRACKER_H

#include <map>
#include "TerrainMemoryMeta.h"
#include "TerrainMemoryMoveMeta.h"
#include "EnclaveKeyDef.h"
#include "TerrainJobResults.h"

class TerrainMemoryTracker
{
public:
	int triangleSizeInGLBytes = 0;						// the number of bytes that a single triangle occupies in OpenGL memory (for example, if one vertex = 8 floats (3 for vertex, 2 for texture UV, for normal -- the amount would be 8, x3 (vertices) x4 (size of a float)
	int maxIndex = 0;									// the value of the top most element in teh map (for 10 elements, this would be 9)
	std::map<int, TerrainMemoryMeta> terrainTracker;	// an ordered map of terrain memory meta information
	void setTriangleSize(int in_triangleSize);			// set the value of triangleSizeInGLBytes
	void jobFlagAsReplaceable(EnclaveKeyDef::EnclaveKey in_collectionKey);		// this function will flag an element in the terrainTracker map as being replaceable
	int insertNewCollection(TerrainJobResults in_terrainJobResults);
	TerrainMemoryMoveMeta removeUnusedReplaceablesAndShift();
	void outputAllElements();
	TerrainMemoryMoveMeta checkForMemoryMovements(TerrainJobResults in_terrainJobResults);
	int getCollectionOffset(EnclaveKeyDef::EnclaveKey in_collectionKey);
	int getNumberOfUnusedReplaceables();
};

#endif
