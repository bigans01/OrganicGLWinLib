#pragma once

#ifndef TERRAINMEMORYMOVEMETA_H
#define TERRAINMEMORYMOVEMETA_H

class TerrainMemoryMoveMeta
{
public:
	int containsMovement = 0;	// determines whether or not there will be movement; 0 is no movement, 1 is movement.
	int byteOffset = 0;		// the offset in bytes for the beginning of the memory movement
	int byteSize = 0;		// the size in bytes of the memory to movement
	int copyBackOffset = 0;	// optionally used; provides the offset to copy back to
};

#endif