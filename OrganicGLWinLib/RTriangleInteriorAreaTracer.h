#pragma once

#ifndef RTRIANGLEINTERIORAREATRACER_H
#define RTRIANGLEINTERIORAREATRACER_H

#include "MassGridArray.h"
#include "RasterCubeTracerBase.h"
#include <glm/glm.hpp>

class RTriangleInteriorAreaTracer : public RasterCubeTracerBase
{
	public:
		void setGridArrayRef(MassGridArray* in_massGridArrayRef);
		void setDownfillCrustBit(short in_downfillCrustBitValue);
		void setUpfillCrustBit(short in_upfillCrustBitValue);
		void setEmptyNormal(glm::vec3 in_emptyNormal);
		void runTrace();
	private:
		short downfillCrustBitValue = 0;
		short upfillCrustBitValue = 0;
		MassGridArray* gridArrayRef = nullptr;
		glm::vec3 tracerEmptyNormal;

};

#endif
