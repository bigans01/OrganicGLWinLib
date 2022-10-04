#include "stdafx.h"
#include "SPolyUtils.h"

Message SPolyUtils::meltSPolySupergroupIntoMessage(SPolySupergroup* in_sPolySupergroupRef)
{
	Message meltedGroupData;
	int numberOfSPolys = in_sPolySupergroupRef->sPolyMap.size();

	// very first value for message will be the number of SPolys.
	meltedGroupData.insertInt(numberOfSPolys);

	// cycle through each SPoly
	for (auto& currentSPoly : in_sPolySupergroupRef->sPolyMap)
	{
		// get the current SPoly ID, insert that as next part of message.
		int currentSPolyID = currentSPoly.first;
		meltedGroupData.insertInt(currentSPolyID);

		// get the number of triangles in the SPoly, and insert that.
		int currentNumberOfPolyTriangles = currentSPoly.second.triangles.size();
		meltedGroupData.insertInt(currentNumberOfPolyTriangles);

		// insert empty normal
		glm::vec3 currentSPolyEmptyNormal = currentSPoly.second.getEmptyNormal();
		ECBPolyPoint convertedEmptyNormal(currentSPolyEmptyNormal.x, currentSPolyEmptyNormal.y, currentSPolyEmptyNormal.z);
		meltedGroupData.insertPoint(convertedEmptyNormal);

		// insert the indicator data.
		meltedGroupData.insertInt(int(currentSPoly.second.sPolyBoundaryIndicator.getIndicatorData()));

		// now, cycle through each triangle; get pointA of each line and insert it into the Message.
		for (auto& currentSTriangle : currentSPoly.second.triangles)
		{
			for (int currentLine = 0; currentLine < 3; currentLine++)
			{
				glm::vec3 currentLinePoint = currentSTriangle.second.triangleLines[currentLine].pointA;
				ECBPolyPoint convertedPoint(currentLinePoint.x, currentLinePoint.y, currentLinePoint.z);
				meltedGroupData.insertPoint(convertedPoint);
			}
		}
	}

	return meltedGroupData;
}

SPolySupergroup SPolyUtils::solidifySupergroupFromMessage(Message* in_messageRef)
{
	// open the message; it should only contain necessary meta data for this operation; all other data should be clipped.
	in_messageRef->open();

	// print out meta data about the SPoly here
	int numberOfSPolys = in_messageRef->readInt();

	SPolySupergroup solidifiedBoundaryGroup;

	if (numberOfSPolys > 0)	// only continue if there is SPoly data to read.
	{
		for (int x = 0; x < numberOfSPolys; x++)
		{


			int currentSPolyID = in_messageRef->readInt();
			//std::cout << "(RJPhasedDeleteBlock): Current liquidated SPoly id: " << currentSPolyID << std::endl;
			int numberOfSTriangles = in_messageRef->readInt();
			//std::cout << "(RJPhasedDeleteBlock): Current number of STriangles in liquidated SPoly: " << numberOfSTriangles << std::endl;

			int oppositeIndicatorData = in_messageRef->readInt();
			ECBPolyPoint solidifiedEmptyNormal = in_messageRef->readPoint();
			//std::cout << "(RJPhasedDeleteBlock): Current empty normal of liquidated SPoly: ";
			//solidifiedEmptyNormal.printPointCoords();
			//std::cout << std::endl;

			//std::cout << "(RJPhasedDeleteBlock): Opposite orientation of liquidated SPoly: ";
			//IndependentUtils::printBoundaryOrientation(currentoppositeIndicatorData);
			//std::cout << std::endl;

			// set up the SPoly; set it's normal and opposite orientation.
			SPoly currentSolidifiedSPoly;
			currentSolidifiedSPoly.setEmptyNormal(OrganicGLWinUtils::convertPolyPointToVec3(solidifiedEmptyNormal));
			
			BoundaryPolyIndicator formedIndicator;
			formedIndicator.setIndicatorData(unsigned char(oppositeIndicatorData));
			currentSolidifiedSPoly.setBoundaryIndicator(formedIndicator);

			// safety: continue reading message, only if STriangles > 0.
			if (numberOfSTriangles > 0)
			{
				for (int y = 0; y < numberOfSTriangles; y++)
				{
					//std::cout << "(RJPhasedDeleteBlock): points for triangle with id " << y << ": " << std::endl;
					glm::vec3 sTrianglePoints[3];
					for (int z = 0; z < 3; z++)
					{
						ECBPolyPoint rawPoint = in_messageRef->readPoint();
						sTrianglePoints[z] = OrganicGLWinUtils::convertPolyPointToVec3(rawPoint);

						//rawPoint.printPointCoords();
						//std::cout << std::endl;
					}

					// form the STriangle from the solidified data; add it to the SPoly.
					STriangle currentSolidifiedSTriangle(sTrianglePoints[0], sTrianglePoints[1], sTrianglePoints[2]);
					currentSolidifiedSPoly.addTriangle(currentSolidifiedSTriangle);
				}
			}

			// finally, add to the supergroup.
			solidifiedBoundaryGroup.insertSPoly(currentSolidifiedSPoly);
		}
	}

	return solidifiedBoundaryGroup;
}

std::vector<OrganicWrappedBBFan> SPolyUtils::produceFansFromSupergroup(SPolySupergroup* in_sPolySupergroupRef)
{
	// Remember: one BB Fan is equivalent to one STriangle in an SPoly.
	// The BB fan's empty normal and alignment must come from the same SPoly that the STriangle originated from.
	std::vector<OrganicWrappedBBFan> returnFans;

	// cycle through each SPoly; we must check to see the number of STriangles in each SPoly, to determine what to do.
	// This is because if the number of triangles is > 6, we will have to split the SPoly into multiple fans.
	for (auto& currentSPoly : in_sPolySupergroupRef->sPolyMap)
	{
		int currentNumberOfSTriangles = currentSPoly.second.triangles.size();

		// the normal operation, almost 100% of the time. There are 6 or less triangles, so 8 points.
		if (currentNumberOfSTriangles < 7)
		{
			auto currentTargetSTriangleBegin = currentSPoly.second.triangles.begin();
			auto currentTargetSTriangleEnd = currentSPoly.second.triangles.end();

			BlockCircuit tempCircuit;

			// the very first triangle in the SPoly, will insert it's 3 points, as we are following the rules of the fan; 
			// we will then go to the next triangle.
			for (int x = 0; x < 3; x++)
			{
				tempCircuit.finalCircuitPoints.insertNewPoint(OrganicGLWinUtils::convertVec3ToPolyPoint(currentTargetSTriangleBegin->second.triangleLines[x].pointA));
			}

			// Now, iterate currentTargetSTriangleBegin by 1, and then continue with the rest of the triangles.
			// The Point to insert in each of these fans should be point A of the 3rd line. So point A of the line at index 2.
			currentTargetSTriangleBegin++;
			for (; currentTargetSTriangleBegin != currentTargetSTriangleEnd; currentTargetSTriangleBegin++)
			{
				tempCircuit.finalCircuitPoints.insertNewPoint(OrganicGLWinUtils::convertVec3ToPolyPoint(currentTargetSTriangleBegin->second.triangleLines[2].pointA));
			}

			// Construct the OrganicWrappedBBFan we will be adding, and then call buildBBFanWithBoundaryIndicator on it.
			auto currentMaterial = currentSPoly.second.getSPolyMaterial();
			auto currentEmptyNormal = OrganicGLWinUtils::convertVec3ToPolyPoint(currentSPoly.second.getEmptyNormal());

			BoundaryPolyIndicator currentIndicator;
			currentIndicator.setIndicatorData(currentSPoly.second.sPolyBoundaryIndicator.getIndicatorData());

			OrganicWrappedBBFan constructedFan;
			constructedFan.buildBBFanWithBoundaryIndicator(&tempCircuit, currentMaterial, currentEmptyNormal, currentIndicator);
			
			returnFans.push_back(constructedFan);
		}
	}
	return returnFans;
}