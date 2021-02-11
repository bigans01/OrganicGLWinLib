#include "stdafx.h"
#include "CutLineWelder.h"
#include "CuttableTriangle.h"

CutLineWelder::CutLineWelder(CuttableTriangle* in_cuttableTriangleRef, CuttingTriangle* in_cuttingTriangleRef, TwoDCrawlingAttempt in_attemptCopy, CutLinePool in_basePool)
{
	cuttableTriangleRef = in_cuttableTriangleRef;
	cuttingTriangleRef = in_cuttingTriangleRef;
	copiedAttempt = in_attemptCopy;
	currentPool = in_basePool;
}