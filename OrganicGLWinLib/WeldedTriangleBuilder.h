#pragma once

#ifndef WELDEDTRIANGLEBUILDER_H
#define WELDEDTRIANGLEBUILDER_H

#include "WeldedLinePool.h"

class WeldedTriangleBuilder
{
public:
	void setWeldedLinePool(WeldedLinePool in_weldedlinePool);
private:
	WeldedLinePool linePool;

};

#endif