#pragma once

#ifndef STRIANGLECUTTER_H
#define STRIANGLECUTTER_H

#include <map>
#include "STriangle.h"
#include "CuttableTriangleContainer.h"
#include "CuttingTriangleManager.h"
#include "STriangleOutputContainer.h"

class STriangleCutter
{
	public:
		void setCuttingParameters(STriangle in_sTriangle, CuttingTriangleManager* in_cuttingTriangleManagerRef);
		void runCuttingSequence();
	private:
		CuttableTriangleContainer cuttableContainer;
		CuttingTriangleManager* cuttingTrianglesRef = nullptr;

};

#endif