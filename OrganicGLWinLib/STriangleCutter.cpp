#include "stdafx.h"
#include "STriangleCutter.h"

void STriangleCutter::setCuttingParameters(STriangle in_sTriangle, CuttingTriangleManager* in_cuttingTriangleManagerRef)
{
	cuttableContainer.buildFirstCuttableTriangle(in_sTriangle);		// build the triangle to cut from the STriangle
	cuttingTrianglesRef = in_cuttingTriangleManagerRef;
}

void STriangleCutter::setDebugLevelForSpecificCuttingTriangle(int in_cuttingTriangleID, PolyDebugLevel in_polyDebugLevel)
{
	specificCuttingTriangleOptionMap[in_cuttingTriangleID] = in_polyDebugLevel;
}

PolyDebugLevel STriangleCutter::getPolyDebugLevelForSpecificCuttingTriangle(int in_cuttingTriangleID)
{
	PolyDebugLevel returnLevel = PolyDebugLevel::NONE;
	auto specificFinder = specificCuttingTriangleOptionMap.find(in_cuttingTriangleID);
	if (specificFinder != specificCuttingTriangleOptionMap.end())	// it was found, so get whatever the debug option is.
	{
		returnLevel = specificFinder->second;
	}
	return returnLevel;
}

void STriangleCutter::runCuttingSequence()
{
	// number of iterations will be equal to the size of the map in the reference CuttingTriangleManager.
	auto cuttingTrianglesBegin = cuttingTrianglesRef->cuttingTriangles.begin();
	auto cuttingTrianglesEnd = cuttingTrianglesRef->cuttingTriangles.end();
	for (; cuttingTrianglesBegin != cuttingTrianglesEnd; cuttingTrianglesBegin++)
	{
		
		// for each iteration, we must go through all of the CuttableTriangles that exist in the CuttableTriangleContainer.
		// in the very first iteration of this loop -- the very beginning of the cuttingSequence -- there should only be 1 CuttingTriangle, which
		// is the one constructed by the call to cuttableContainer.buildFirstCuttableTriangle.

		STriangleOutputContainer outputsForCurrentCuttingTriangle;
		auto currentTriangleToCutBegin = cuttableContainer.cuttableTriangleMap.begin();
		auto currentTriangleToCutEnd = cuttableContainer.cuttableTriangleMap.end();
		for (; currentTriangleToCutBegin != currentTriangleToCutEnd; currentTriangleToCutBegin++)
		{
			currentTriangleToCutBegin->second.compareAgainstCuttingTriangle(&cuttingTrianglesBegin->second, 
																			cuttingTrianglesBegin->first, 
																			getPolyDebugLevelForSpecificCuttingTriangle(cuttingTrianglesBegin->first));

			// optional: print out the contents before output triangles are constructed.
			std::cout << "++++ Printing out registry for the cuttable triangle: " << std::endl;
			currentTriangleToCutBegin->second.printCuttableLineIntersections();
			std::cout << "++++ Printing out registry for the cutting triangle: " << std::endl;
			cuttingTrianglesBegin->second.printCuttingLineIntersections();

			// fetch whatever the result of the comparison was -- if it is completely eliminated, the size of the vector will be 0.
			// Otherwise, it's size will be >= 1.
			outputsForCurrentCuttingTriangle.insertOutputSTriangles(&currentTriangleToCutBegin->second.outputTriangles);	
		}

		// rebuild/analyze the cuttableTriangleMap in the CuttableTriangleContainer, once we've gone through all of the CuttableTriangles in it.
		cuttableContainer.rebuildCuttableTriangleMapFromContainer(&outputsForCurrentCuttingTriangle);
		//cuttableContainer.printCuttableTriangles();

		// reset the CuttingTriangle that was used in this iteration.
		cuttingTrianglesBegin->second.reset();
	}


	// if the number of CuttableTriangles in the cuttableContainer's cuttableTriangleMap is 0, it means everything got clipped, and the STriangle
	// was completely eliminated. Otherwise, it will remain.
}