#pragma once

#ifndef CUTLINEWELDER_H
#define CUTLINEWELDER_H

#include "CuttingTriangle.h"
#include "TwoDCrawlingAttempt.h"
#include "CutLinePool.h"

class CuttableTriangle;
class CutLineWelder
{
	public:
		CutLineWelder(CuttableTriangle* in_cuttableTriangleRef, CuttingTriangle* in_cuttingTriangleRef, TwoDCrawlingAttempt in_crawlingAttempt, CutLinePool in_basePool);
	private:
		CuttableTriangle* cuttableTriangleRef = nullptr;
		CuttingTriangle* cuttingTriangleRef = nullptr;
		TwoDCrawlingAttempt copiedAttempt;
		CutLinePool currentPool;
};

#endif
