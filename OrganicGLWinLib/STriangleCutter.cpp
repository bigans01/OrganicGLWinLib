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

bool STriangleCutter::runCuttingSequence()
{
	// the result of whether or not it was completely "destroyed"
	bool wasTriangleDestroyedDuringSequence = false;

	// number of iterations will be equal to the size of the map in the reference CuttingTriangleManager.
	auto cuttingTrianglesBegin = cuttingTrianglesRef->cuttingTriangles.begin();
	auto cuttingTrianglesEnd = cuttingTrianglesRef->cuttingTriangles.end();
	for (; cuttingTrianglesBegin != cuttingTrianglesEnd; cuttingTrianglesBegin++)
	{
		
		// for each iteration, we must go through all of the CuttableTriangles that exist in the CuttableTriangleContainer.
		// in the very first iteration of this loop -- the very beginning of the cuttingSequence -- there should only be 1 CuttingTriangle, which
		// is the one constructed by the call to cuttableContainer.buildFirstCuttableTriangle.

		std::cout << "!!!! Next pass begin. " << std::endl;

		STriangleOutputContainer outputsForCurrentCuttingTriangle;
		auto currentTriangleToCutBegin = cuttableContainer.cuttableTriangleMap.begin();
		auto currentTriangleToCutEnd = cuttableContainer.cuttableTriangleMap.end();
		for (; currentTriangleToCutBegin != currentTriangleToCutEnd; currentTriangleToCutBegin++)
		{
			std::cout << "!! START: Comparing against CuttableTriangle, with ID: " << currentTriangleToCutBegin->first << std::endl;
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

			// reset the CuttingTriangle that was used in this iteration.
			cuttingTrianglesBegin->second.reset();
			std::cout << "!! END: Comparing against CuttableTriangle, with ID: " << currentTriangleToCutBegin->first << std::endl;
		}

		std::cout << "!!!! Next pass end. " << std::endl;

		// rebuild/analyze the cuttableTriangleMap in the CuttableTriangleContainer, once we've gone through all of the CuttableTriangles in it.
		cuttableContainer.rebuildCuttableTriangleMapFromContainer(&outputsForCurrentCuttingTriangle);
		std::cout << "||||||||||||||||| printing out cuttable triangles, after this pass: (Cutting Triangle ID was: " << cuttingTrianglesBegin->first << ")" << std::endl;
		cuttableContainer.printCuttableTriangles();
		int passWait = 3; 
		std::cin >> passWait;

		// reset the CuttingTriangle that was used in this iteration.
		//cuttingTrianglesBegin->second.reset();
	}

	// if the number of CuttableTriangles in the cuttableContainer's cuttableTriangleMap is 0, it means everything got clipped, and the STriangle
	// was completely eliminated. Otherwise, it will remain.
	if (cuttableContainer.cuttableTriangleMap.size() == 0)
	{
		wasTriangleDestroyedDuringSequence = true;
		std::cout << "#########################::::::" << std::endl;
		std::cout << ":::::::::::: NOTICE: cutting sequence flagged the STriangle as being completely destroyed!" << std::endl;
		std::cout << "#########################::::::" << std::endl;
	}
	else if (cuttableContainer.cuttableTriangleMap.size() != 0)
	{
		auto resultStart = cuttableContainer.cuttableTriangleMap.begin();
		auto resultEnd = cuttableContainer.cuttableTriangleMap.end();
		for (; resultStart != resultEnd; resultStart++)
		{
			std::cout << "---> Resulting triangle points: " << std::endl;
			resultStart->second.printCuttableTrianglePoints();
		}
	}
	std::cout << "########################--------------------->> end of call to STriangleCutter::runCuttingSequence(). " << std::endl;
	return wasTriangleDestroyedDuringSequence;
}