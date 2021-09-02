#include "stdafx.h"
#include "MassZoneBox.h"

void MassZoneBox::insertNewBoundary(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, MassZoneBoxBoundary in_massZoneBoxBoundary, SPolyDOSet in_sPolyDOSet)
{
	boxBoundaries[in_massZoneBoxBoundaryOrientation] = in_massZoneBoxBoundary;
	boxBoundaries[in_massZoneBoxBoundaryOrientation].setBoundarySPolyInPolySet();	// must be called, or program will crash. The boundary SPoly ref should only be set
																					// after the MassZoneBoxBoundary has been copied into the map, since it will correctly point to that newly
																					// allocated memory.
	boxBoundaries[in_massZoneBoxBoundaryOrientation].setDebugOptionsInSPoly(std::move(in_sPolyDOSet));
	boxBoundaries[in_massZoneBoxBoundaryOrientation].setSPolySetEmptyNormal();		// sets the empty normal for the MassZoneBoxBoundarySPolySet
	//std::cout << "!!! Size of box boundaries is now: " << boxBoundaries.size() << std::endl;
};

void MassZoneBox::printBoundaryLineCounts()
{
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			std::cout << "Counts for boundary NEG_Z: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			std::cout << "Counts for boundary POS_X: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			std::cout << "Counts for boundary POS_Z: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			std::cout << "Counts for boundary NEG_X: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			std::cout << "Counts for boundary POS_Y: ";
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			std::cout << "Counts for boundary NEG_Y: ";
		}
		std::cout << boxBoundariesBegin->second.getBoundarySPolyBorderLineCounts() << std::endl;
	}
}

void MassZoneBox::printBoundaries()
{
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			std::cout << "Points for boundary NEG_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			std::cout << "Points for boundary POS_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			std::cout << "Points for boundary POS_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			std::cout << "Points for boundary NEG_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			std::cout << "Points for boundary POS_Y: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			std::cout << "Points for boundary NEG_Y: " << std::endl;
		}

		boxBoundariesBegin->second.printPoints();

	}

	std::cout << "Finished printing MassZoneBoxBoundaries. " << std::endl;
	int finishedPrintVal = 3;
	std::cin >> finishedPrintVal;
}

void MassZoneBox::runSPolyBasedSubZoneAgainstBoundaries(MassSubZone* in_massSubZoneRef)
{
	//std::cout << "!!! border lines of passed in parameter: " << in_massSubZoneRef->sPolyCopy.borderLines.size() << std::endl;
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			//std::cout << "> Comparing against NEG_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			//std::cout << "> Comparing against POS_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			//std::cout << "> Comparing against POS_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			//std::cout << "> Comparing against NEG_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			//std::cout << "> Comparing against POS_Y: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			//std::cout << ">++++++++++++++++++++++++++++++++++++++++++++++ Comparing against NEG_Y: " << std::endl;
		}
		boxBoundariesBegin->second.compareSPolyBasedSubZoneSPolyToBoundarySPolySet(&in_massSubZoneRef->sPolyCopy);
		//std::cout << ">++++++++++++++++++++++++++++++++++++++++++++++ Compare against ended. " << std::endl;
	}
}

std::set<MassZoneBoxBoundaryOrientation> MassZoneBox::generateTouchedBoxFacesList(MassZoneBoxType in_massZoneBoxType)
{
	std::set<MassZoneBoxBoundaryOrientation> generatedTouchedList;
	PolyLogger touchedLogger;
	touchedLogger.setDebugLevel(generatedTouchedFaceDebugLevel);
	auto currentBoundaryBegin = boxBoundaries.begin();
	auto currentBoundaryEnd = boxBoundaries.end();
	for (; currentBoundaryBegin != currentBoundaryEnd; currentBoundaryBegin++)
	{
		// only continue if there's actually 
		// an SPoly that was produced in the MassZoneBoxBoundarySPolySet.
		// std::cout << "!! Entering next boundary.." << std::endl;
		switch (currentBoundaryBegin->first)
		{
			case MassZoneBoxBoundaryOrientation::NEG_X:
			{
				//std::cout << "(MassZoneBox): Found box Boundary NEG_X" << std::endl;
				touchedLogger.log("(MassZoneBox): Found box Boundary NEG_X", "\n");
				break;
			}
			case MassZoneBoxBoundaryOrientation::NEG_Z:
			{
				//std::cout << "(MassZoneBox): Found box Boundary NEG_Z" << std::endl;
				touchedLogger.log("(MassZoneBox): Found box Boundary NEG_Z", "\n");
				break;
			}
			case MassZoneBoxBoundaryOrientation::POS_X:
			{
				//std::cout << "(MassZoneBox): Found box Boundary POS_X" << std::endl;
				touchedLogger.log("(MassZoneBox): Found box Boundary POS_X", "\n");
				break;
			}
			case MassZoneBoxBoundaryOrientation::POS_Z:
			{
				//std::cout << "(MassZoneBox): Found box Boundary POS_Z" << std::endl;
				touchedLogger.log("(MassZoneBox): Found box Boundary POS_Z", "\n");
				break;
			}
			case MassZoneBoxBoundaryOrientation::POS_Y:
			{
				//std::cout << "(MassZoneBox): Found box Boundary POS_Y" << std::endl;
				touchedLogger.log("(MassZoneBox): Found box Boundary POS_Y", "\n");
				break;
			}
			case MassZoneBoxBoundaryOrientation::NEG_Y:
			{
				//std::cout << "(MassZoneBox): Found box Boundary NEG_Y" << std::endl;
				touchedLogger.log("(MassZoneBox): Found box Boundary NEG_Y", "\n");
				break;
			}
		}

		if (!currentBoundaryBegin->second.boundaryPolySet.boundarySPolySG.sPolyMap.empty())	
		{
			//std::cout << "!! Found produced SPolys to check.." << std::endl;

			auto producedBorderSPolysBegin = currentBoundaryBegin->second.boundaryPolySet.boundarySPolySG.sPolyMap.begin();
			auto producedBorderSPolysEnd = currentBoundaryBegin->second.boundaryPolySet.boundarySPolySG.sPolyMap.end();
			for (; producedBorderSPolysBegin != producedBorderSPolysEnd; producedBorderSPolysBegin++)
			{
				auto currentLinesBegin = producedBorderSPolysBegin->second.borderLines.begin();
				auto currentLinesEnd = producedBorderSPolysBegin->second.borderLines.end();
				for (; currentLinesBegin != currentLinesEnd; currentLinesBegin++)
				{
					glm::vec3 currentPoint = currentLinesBegin->second.pointA;
					ECBPolyPoint convertedPoint(currentPoint.x, currentPoint.y, currentPoint.z);

					//std::cout << "Converted point is: " << convertedPoint.x << ", " << convertedPoint.y << ", " << convertedPoint.z << std::endl;

					ECBPPOrientationResults pointOrientation;
					switch (in_massZoneBoxType)
					{
						case MassZoneBoxType::BLOCK:
						{
							BlockBorderLineList blockBorders;
							pointOrientation = IndependentUtils::GetPointOrientation(convertedPoint, &blockBorders);
							break;
						};
						case MassZoneBoxType::ENCLAVE:
						{
							EnclaveBorderLineList enclaveBorders;
							pointOrientation = IndependentUtils::GetEnclavePointOrientation(convertedPoint, &enclaveBorders);
							if (pointOrientation.otype == ECBPPOrientations::LINE)
							{
								if (touchedLogger.isLoggingSet())
								{
									std::cout << "(MassZoneBox): ";
									IndependentUtils::printOrientationEnum(pointOrientation.osubtype);
								}
							}
							break;
						}
						case MassZoneBoxType::COLLECTION:
						{
							EnclaveKeyDef::EnclaveKey tempKey(0, 0, 0);
							ECBBorderLineList collectionBorders = IndependentUtils::determineBorderLines(tempKey);
							pointOrientation = IndependentUtils::GetBlueprintPointOrientation(convertedPoint, &collectionBorders);
							break;
						}
					}

					// get the face list
					BorderDataMap bdMap;
					BorderMDFaceList pointFaceList = IndependentUtils::getFaceList(pointOrientation, &bdMap);

					for (int x = 0; x < 3; x++)
					{
						if (pointFaceList.faceList[x] != ECBPPOrientations::NOVAL)
						{
							MassZoneBoxBoundaryOrientation currentOrientation = convertPointOrientationToBoundaryOrientation(pointFaceList.faceList[x]);
							generatedTouchedList.insert(currentOrientation);
						}
					}
				}
			}
		}
	}

	if (touchedLogger.isLoggingSet())
	{
		std::cout << "(MassZoneBox): +++++++++++++ Printing out values of the generatedTouchedList (size is " << generatedTouchedList.size() << std::endl;
		auto listBegin = generatedTouchedList.begin();
		auto listEnd = generatedTouchedList.end();
		for (; listBegin != listEnd; listBegin++)
		{
			switch (*listBegin)
			{
			case MassZoneBoxBoundaryOrientation::NEG_X:
			{
				std::cout << "(MassZoneBox): Found touched face NEG_X" << std::endl;
				break;
			}
			case MassZoneBoxBoundaryOrientation::NEG_Z:
			{
				std::cout << "(MassZoneBox): Found touched face NEG_Z" << std::endl;
				break;
			}
			case MassZoneBoxBoundaryOrientation::POS_X:
			{
				std::cout << "(MassZoneBox): Found touched face POS_X" << std::endl;
				break;
			}
			case MassZoneBoxBoundaryOrientation::POS_Z:
			{
				std::cout << "(MassZoneBox): Found touched face POS_Z" << std::endl;
				break;
			}
			case MassZoneBoxBoundaryOrientation::POS_Y:
			{
				std::cout << "(MassZoneBox): Found touched face POS_Y" << std::endl;
				break;
			}
			case MassZoneBoxBoundaryOrientation::NEG_Y:
			{
				std::cout << "(MassZoneBox): Found touched face NEG_Y" << std::endl;
				break;
			}
			}
		}
	}

	return generatedTouchedList;
}

void MassZoneBox::runFirstTertiaryProductionPass(std::set<MassZoneBoxBoundaryOrientation> in_orientationSet,
												 std::map<MassZoneBoxBoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef)
{
	auto orientationSetBegin = in_orientationSet.begin();
	auto orientationSetEnd = in_orientationSet.end();
	for (; orientationSetBegin != orientationSetEnd; orientationSetBegin++)
	{
		if (boxBoundaries[*orientationSetBegin].boundaryPolySet.wasLineProducedByReactor == false)
		{
			switch (*orientationSetBegin)
			{
				case MassZoneBoxBoundaryOrientation::NEG_X:
				{
					//std::cout << "NEG_X";
					break;
				}
				case MassZoneBoxBoundaryOrientation::NEG_Z:
				{
					//std::cout << "NEG_Z";
					break;
				}
				case MassZoneBoxBoundaryOrientation::POS_X:
				{
					//std::cout << "POS_X";
					break;
				}
				case MassZoneBoxBoundaryOrientation::POS_Z:
				{
					//std::cout << "POS_Z";
					break;
				}
				case MassZoneBoxBoundaryOrientation::POS_Y:
				{
					//std::cout << "POS_Y";
					break;
				}
				case MassZoneBoxBoundaryOrientation::NEG_Y:
				{
					//std::cout << "NEG_Y";
					break;
				}
			}
			//std::cout << " needs to have its face produced (tertiary SPoly, 1st pass)" << std::endl;
			SPolySupergroup tempGroup;
			tempGroup.insertSPoly(*boxBoundaries[*orientationSetBegin].boundaryPolySet.boundarySPolyRef);
			(*in_outputSuperGroupsMapRef)[*orientationSetBegin] = tempGroup;
		}
	}
}

void MassZoneBox::setContestedCategorizedLineAnalysisFlagInBoundaries()
{
	auto boundariesBegin = boxBoundaries.begin();
	auto boundariesEnd = boxBoundaries.end();
	for (; boundariesBegin != boundariesEnd; boundariesBegin++)
	{
		boundariesBegin->second.boundaryPolySet.isContestedCategorizedLineAnalysisEnabled = true;
	}
}

void MassZoneBox::setTouchedGenerationDebugLevel(PolyDebugLevel in_debugLevel)
{
	generatedTouchedFaceDebugLevel = in_debugLevel;
}

MassZoneBoxBoundaryOrientation MassZoneBox::convertPointOrientationToBoundaryOrientation(ECBPPOrientations in_pointOrientation)
{
	MassZoneBoxBoundaryOrientation returnOrientation;
	switch (in_pointOrientation)
	{
		case ECBPPOrientations::WESTFACE:
		{
			returnOrientation = MassZoneBoxBoundaryOrientation::NEG_X;
			break;
		}	
		case ECBPPOrientations::NORTHFACE:
		{
			returnOrientation = MassZoneBoxBoundaryOrientation::NEG_Z;
			break;
		}
		case ECBPPOrientations::EASTFACE:
		{
			returnOrientation = MassZoneBoxBoundaryOrientation::POS_X;
			break;
		}
		case ECBPPOrientations::SOUTHFACE:
		{
			returnOrientation = MassZoneBoxBoundaryOrientation::POS_Z;
			break;
		}
		case ECBPPOrientations::TOPFACE:
		{
			returnOrientation = MassZoneBoxBoundaryOrientation::POS_Y;
			break;
		}
		case ECBPPOrientations::BOTTOMFACE:
		{
			returnOrientation = MassZoneBoxBoundaryOrientation::NEG_Y;
			break;
		}
	}
	return returnOrientation;
}

void MassZoneBox::printCategorizedLinesInBoundaries()
{
	std::cout << "################################# Printing categorized lines in each boundary: " << std::endl;
	auto boxBoundariesBegin = boxBoundaries.begin();
	auto boxBoundariesEnd = boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			std::cout << ">>>>>>> Categorized lines for NEG_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			std::cout << ">>>>>>> Categorized lines for POS_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			std::cout << ">>>>>>> Categorized lines for POS_Z: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			std::cout << ">>>>>>> Categorized lines for NEG_X: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			std::cout << ">>>>>>> Categorized lines for POS_Y: " << std::endl;
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			std::cout << ">>>>>>> Categorized lines for NEG_Y: " << std::endl;
		}
		boxBoundariesBegin->second.printBoundarySPolyCategorizedLines();
	}
}