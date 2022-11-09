#pragma once

#ifndef TERRAINMEMORYMETA_H
#define TERRAINMEMORYMETA_H


class TerrainMemoryMeta
{
public:
	int replaceable = 1;	// indicates if the collection may be replaced. default will  be "1", indicating not replaceable.
	EnclaveKeyDef::EnclaveKey collectionKey;	// the key of the collection being tracked
	int byteOffsetBegin = 0; // the exact location in GL memory where this collection's data begins, measured as a byte offset
	int byteSize = 0;        // the size in bytes of the render collection to render
	int byteOffsetEnd = 0;   // the location where the byte offset ends
};

#endif
