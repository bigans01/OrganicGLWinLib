#pragma once

#ifndef OCTANTCORNER_H
#define OCTANTCORNER_H

enum class OctantCorner
{
	NONE,
	CORNER_NEGX_NEGY_NEGZ,	// aka lower SW
	CORNER_NEGX_NEGY_POSZ,	// "   lower NW
	CORNER_POSX_NEGY_POSZ,	// "   lower NE
	CORNER_POSX_NEGY_NEGZ,	// "   lower SE
	CORNER_NEGX_POSY_NEGZ,	// "   upper SW
	CORNER_NEGX_POSY_POSZ,	// "   upper NW
	CORNER_POSX_POSY_POSZ,	// "   upper NE
	CORNER_POSX_POSY_NEGZ   // "   upper SE
};

#endif
