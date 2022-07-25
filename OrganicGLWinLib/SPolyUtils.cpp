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

		// insert the boundary
		meltedGroupData.insertInt(IndependentUtils::convertBoundaryOrientationToInt(currentSPoly.second.sPolyBoundaryIndicator.getBoundaryIndicatorValue()));

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

			int oppositeOrientation = in_messageRef->readInt();
			ECBPolyPoint solidifiedEmptyNormal = in_messageRef->readPoint();
			//std::cout << "(RJPhasedDeleteBlock): Current empty normal of liquidated SPoly: ";
			//solidifiedEmptyNormal.printPointCoords();
			//std::cout << std::endl;

			//std::cout << "(RJPhasedDeleteBlock): Opposite orientation of liquidated SPoly: ";
			BoundaryOrientation currentOppositeOrientation = IndependentUtils::convertIntToBoundaryOrientation(oppositeOrientation);
			//IndependentUtils::printBoundaryOrientation(currentOppositeOrientation);
			//std::cout << std::endl;

			// set up the SPoly; set it's normal and opposite orientation.
			SPoly currentSolidifiedSPoly;
			currentSolidifiedSPoly.setEmptyNormal(OrganicGLWinUtils::convertPolyPointToVec3(solidifiedEmptyNormal));
			currentSolidifiedSPoly.setBoundaryIndicatorOrientation(currentOppositeOrientation);

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

