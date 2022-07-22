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