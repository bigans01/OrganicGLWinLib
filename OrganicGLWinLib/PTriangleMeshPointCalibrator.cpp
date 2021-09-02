#include "stdafx.h"
#include "PTriangleMeshPointCalibrator.h"

void PTriangleMeshPointCalibrator::determineCalibrationGridLimit(float in_dimLimit, float in_cellLength, int in_cellsPerDimension)
{
	int maxLimitIndex = in_cellsPerDimension - 1;
	float cellCenterOffset = in_cellLength / 2;
	maxCalibrationGridLimit = IndependentUtils::roundToHundredth((maxLimitIndex * in_cellLength) + cellCenterOffset);
	minCalibrationGridLimit = IndependentUtils::roundToHundredth(in_dimLimit - maxCalibrationGridLimit);
}

void PTriangleMeshPointCalibrator::calibratePointsInArray()
{
	for (int x = 0; x < translatorPointArrayRef->arraySize; x++)
	{
		calibratePoint(&translatorPointArrayRef->collisionPoints[x]);
	}
}

void PTriangleMeshPointCalibrator::applyCalibration(RCollisionPoint* in_collisionPointRef, MassGridArrayCellLimit in_cellLimit)
{
	//glm::vec3 testVec3 = in_collisionPointRef->currentValue;

	if (in_cellLimit.applyXValue == 1)
	{
		if (in_collisionPointRef->currentValue.x == minCalibrationGridLimit)
		{
			//testVec3.x = 0.0f;
			in_collisionPointRef->currentValue.x = 0.0f;

		}
		else if (in_collisionPointRef->currentValue.x == maxCalibrationGridLimit)
		{
			//testVec3.x = 4.0f;
			in_collisionPointRef->currentValue.x = 4.0f;
		}
	}

	if (in_cellLimit.applyYValue == 1)
	{
		if (in_collisionPointRef->currentValue.y == minCalibrationGridLimit)
		{
			//testVec3.y = 0.0f;
			in_collisionPointRef->currentValue.y = 0.0f;
		}
		else if (in_collisionPointRef->currentValue.y == maxCalibrationGridLimit)
		{
			//testVec3.y = 4.0f;
			in_collisionPointRef->currentValue.y = 4.0f;
		}
	}

	if (in_cellLimit.applyZValue == 1)
	{
		if (in_collisionPointRef->currentValue.z == minCalibrationGridLimit)
		{
			//testVec3.z = 0.0f;
			in_collisionPointRef->currentValue.z = 0.0f;
		}
		else if (in_collisionPointRef->currentValue.z == maxCalibrationGridLimit)
		{
			//testVec3.z = 4.0f;
			in_collisionPointRef->currentValue.z = 4.0f;
		}
	}

	//in_collisionPointRef->currentValue = testVec3;

	//std::cout << "(PTriangleMeshCalibrator): --> newly calibrated point is: " << in_collisionPointRef->currentValue.x << ", " << in_collisionPointRef->currentValue.y << ", " << in_collisionPointRef->currentValue.z << std::endl;
}

void PTriangleMeshPointCalibrator::calibratePoint(RCollisionPoint* in_collisionPointRef)
{
	glm::vec3* currentValueRef = &in_collisionPointRef->currentValue;

	// ||||||||||||||||||||||||||||||||||||||||||||||||||
	// ************************ CORNER LIMITS
	// ||||||||||||||||||||||||||||||||||||||||||||||||||

	if (currentValueRef->x == dynamicGridCellBorders.corner_LowerNW.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_LowerNW.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_LowerNW.limit.z)		// Lower NW
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Lower NW corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNW;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_LowerNW);
	}
	else if (currentValueRef->x == dynamicGridCellBorders.corner_LowerSW.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_LowerSW.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_LowerSW.limit.z)		// Lower SW
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Lower SW corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSW;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_LowerSW);
	}
	else if (currentValueRef->x == dynamicGridCellBorders.corner_LowerSE.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_LowerSE.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_LowerSE.limit.z)		// Lower SE
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Lower SE corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_LOWERSE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_LowerSE);
	}
	else if (currentValueRef->x == dynamicGridCellBorders.corner_LowerNE.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_LowerNE.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_LowerNE.limit.z)		// Lower NE
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Lower NE corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_LOWERNE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_LowerNE);
	}
	else if (currentValueRef->x == dynamicGridCellBorders.corner_UpperNW.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_UpperNW.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_UpperNW.limit.z)		// Upper NW
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Upper NW corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNW;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_UpperNW);
	}
	else if (currentValueRef->x == dynamicGridCellBorders.corner_UpperSW.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_UpperSW.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_UpperSW.limit.z)		// Upper SW
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Upper SW corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSW;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_UpperSW);
	}
	else if (currentValueRef->x == dynamicGridCellBorders.corner_UpperSE.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_UpperSE.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_UpperSE.limit.z)		// Upper SE
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Upper SE corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_UPPERSE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_UpperSE);
	}
	else if (currentValueRef->x == dynamicGridCellBorders.corner_UpperNE.limit.x	&&		currentValueRef->y == dynamicGridCellBorders.corner_UpperNE.limit.y	&&		currentValueRef->z == dynamicGridCellBorders.corner_UpperNE.limit.z)		// Upper SE
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Upper NE corner hit" << std::endl;
		//returnResults.otype = ECBPPOrientations::CORNER;
		//returnResults.osubtype = ECBPPOrientations::CORNER_UPPERNE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.corner_UpperNE);
	}

	// ||||||||||||||||||||||||||||||||||||||||||||||||||
	// ************************ Z LINE LIMITS
	// ||||||||||||||||||||||||||||||||||||||||||||||||||
	// >>>>> Lower West Z line
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_LowerNW.limit.x)
		&&
		(currentValueRef->y == dynamicGridCellBorders.corner_LowerNW.limit.y)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_LowerNW.limit.z	&& currentValueRef->z < dynamicGridCellBorders.corner_LowerSW.limit.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWERWEST;
		//std::cout << "(PTriangleMeshPointCalibrator): Z lines -- Point is at lower west line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Zaxis_lowerWest);
	}

	// >>>>> Upper West line check
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_UpperNW.limit.x)
		&&
		(currentValueRef->y == dynamicGridCellBorders.corner_UpperNW.limit.y)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_UpperNW.limit.z	&& currentValueRef->z < dynamicGridCellBorders.corner_UpperSW.limit.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPERWEST;
		//std::cout << "(PTriangleMeshPointCalibrator): Z lines -- Point is at upper west line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Zaxis_upperWest);
	}

	// >>>>> Upper East line check
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_UpperNE.limit.x)
		&&
		(currentValueRef->y == dynamicGridCellBorders.corner_UpperNE.limit.y)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_UpperNE.limit.z	&& currentValueRef->z < dynamicGridCellBorders.corner_UpperSE.limit.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::ZAXIS_UPPEREAST;
		//std::cout << "(PTriangleMeshPointCalibrator): Z lines -- Point is at upper east line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Zaxis_upperEast);
	}
	// >>>>> Lower East line check
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_LowerNE.limit.x)
		&&
		(currentValueRef->y == dynamicGridCellBorders.corner_LowerNE.limit.y)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_LowerNE.limit.z	&& currentValueRef->z < dynamicGridCellBorders.corner_LowerSE.limit.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::ZAXIS_LOWEREAST;
		//std::cout << "(PTriangleMeshPointCalibrator): Z lines -- Point is at lower east line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Zaxis_lowerEast);
	}





	// ||||||||||||||||||||||||||||||||||||||||||||||||||
	// ************************ Y LINE LIMITS
	// ||||||||||||||||||||||||||||||||||||||||||||||||||

	// North East line
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_LowerNE.limit.x)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_LowerNE.limit.z)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerNE.limit.y	&& currentValueRef->y < dynamicGridCellBorders.corner_UpperNE.limit.y)
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHEAST;
		//std::cout << "(PTriangleMeshPointCalibrator): Y lines -- Point is at north east line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Yaxis_northEast);
	}

	// South East line
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_LowerSE.limit.x)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_LowerSE.limit.z)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerSE.limit.y	&& currentValueRef->y < dynamicGridCellBorders.corner_UpperSE.limit.y)
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHEAST;
		//std::cout << "(PTriangleMeshPointCalibrator): Y lines -- Point is at south east line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Yaxis_southEast);
	}

	// South West line
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_LowerSW.limit.x)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_LowerSW.limit.z)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerSW.limit.y	&& currentValueRef->y < dynamicGridCellBorders.corner_UpperSW.limit.y)
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::YAXIS_SOUTHWEST;
		//std::cout << "(PTriangleMeshPointCalibrator): Y lines -- Point is at south west line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Yaxis_southWest);
	}

	// North West line
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_LowerNW.limit.x)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_LowerNW.limit.z)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerNW.limit.y	&& currentValueRef->y < dynamicGridCellBorders.corner_UpperNW.limit.y)
		)
	{
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::YAXIS_NORTHWEST;
		//std::cout << "(PTriangleMeshPointCalibrator): Y lines -- Point is at north west line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Yaxis_northWest);
	}

	// ||||||||||||||||||||||||||||||||||||||||||||||||||
	// ************************ X LINE LIMITS
	// ||||||||||||||||||||||||||||||||||||||||||||||||||


	else if ((currentValueRef->y == dynamicGridCellBorders.corner_LowerNW.limit.y)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_LowerNW.limit.z)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_LowerNW.limit.x	&&		currentValueRef->x < dynamicGridCellBorders.corner_LowerNE.limit.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(dynamicGridCellBorders.Xaxis_lowerNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERNORTH;
		//std::cout << "(PTriangleMeshPointCalibrator): X lines -- Point is at lower north line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Xaxis_lowerNorth);

	}
	// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((currentValueRef->y == dynamicGridCellBorders.corner_LowerSW.limit.y)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_LowerSW.limit.z)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_LowerSW.limit.x	&&	currentValueRef->x < dynamicGridCellBorders.corner_LowerSE.limit.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(dynamicGridCellBorders.Xaxis_lowerSouth, in_originPoint, in_pointToCheck);	// get the shifting key

		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::XAXIS_LOWERSOUTH;
		//std::cout << "(PTriangleMeshPointCalibrator): X lines -- Point is at lower south line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Xaxis_lowerSouth);
	}
	// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((currentValueRef->y == dynamicGridCellBorders.corner_UpperSW.limit.y)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_UpperSW.limit.z)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_UpperSW.limit.x	&& currentValueRef->x < dynamicGridCellBorders.corner_UpperSE.limit.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(dynamicGridCellBorders.Xaxis_upperSouth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERSOUTH;
		//std::cout << "Point is at upper south line....." << std::endl;
		//std::cout << "(PTriangleMeshPointCalibrator): X lines -- Point is at upper south line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Xaxis_upperSouth);
	}
	// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	else if ((currentValueRef->y == dynamicGridCellBorders.corner_UpperNW.limit.y)
		&&
		(currentValueRef->z == dynamicGridCellBorders.corner_UpperNW.limit.z)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_UpperNW.limit.x	&& currentValueRef->x < dynamicGridCellBorders.corner_UpperNE.limit.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
		)
	{
		//newKey = OrganicUtils::getBorderShiftResult(dynamicGridCellBorders.Xaxis_upperNorth, in_originPoint, in_pointToCheck);	// get the shifting key
		//*in_KeyPtr = OrganicUtils::addEnclaveKeys(*in_KeyPtr, newKey);
		//returnResults.otype = ECBPPOrientations::LINE;
		//returnResults.osubtype = ECBPPOrientations::XAXIS_UPPERNORTH;
		//std::cout << "(PTriangleMeshPointCalibrator): X lines -- Point is at upper north line " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.Xaxis_upperNorth);
	}

	// ||||||||||||||||||||||||||||||||||||||||||||||||||
	// ************************ FACE LIMITS
	// ||||||||||||||||||||||||||||||||||||||||||||||||||
	// West face check
	else if ((currentValueRef->x == minCalibrationGridLimit)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerNW.limit.y)
		&&
		(currentValueRef->y < dynamicGridCellBorders.corner_UpperNW.limit.y)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_UpperNW.limit.z)
		&&
		(currentValueRef->z < dynamicGridCellBorders.corner_UpperSW.limit.z)

	)
	{
		//std::cout << "(PTriangleMeshPointCalibrator): West face found. " << std::endl;
		//returnResults.otype = ECBPPOrientations::FACE;
		//returnResults.osubtype = ECBPPOrientations::WESTFACE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.WestFace);
	}

	// North face check
	else if ((currentValueRef->z == minCalibrationGridLimit)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_LowerNW.limit.x)
		&&
		(currentValueRef->x < dynamicGridCellBorders.corner_LowerNE.limit.x)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerNW.limit.y)
		&&
		(currentValueRef->y < dynamicGridCellBorders.corner_UpperNW.limit.y)
	)
	{
		//std::cout << "(PTriangleMeshPointCalibrator): North face found. " << std::endl;
		//returnResults.otype = ECBPPOrientations::FACE;
		//returnResults.osubtype = ECBPPOrientations::NORTHFACE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.NorthFace);
	}

	// East face check
	else if ((currentValueRef->x == dynamicGridCellBorders.corner_LowerNE.limit.x)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerNW.limit.y)
		&&
		(currentValueRef->y < dynamicGridCellBorders.corner_UpperNW.limit.y)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_UpperNW.limit.z)
		&&
		(currentValueRef->z < dynamicGridCellBorders.corner_UpperSW.limit.z)
	)
	{
		//std::cout << "(PTriangleMeshPointCalibrator): East face found. " << std::endl;
		//returnResults.otype = ECBPPOrientations::FACE;
		//returnResults.osubtype = ECBPPOrientations::EASTFACE;
		//std::cout << "EAST FACE hit! " << std::endl;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.EastFace);
	}

	// South face check
	else if ((currentValueRef->z == dynamicGridCellBorders.corner_LowerSE.limit.z)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_LowerNW.limit.x)
		&&
		(currentValueRef->x < dynamicGridCellBorders.corner_LowerNE.limit.x)
		&&
		(currentValueRef->y > dynamicGridCellBorders.corner_LowerNW.limit.y)
		&&
		(currentValueRef->y < dynamicGridCellBorders.corner_UpperNW.limit.y)
	)
	{
		//std::cout << "(PTriangleMeshPointCalibrator): South face found. " << std::endl;
		//returnResults.otype = ECBPPOrientations::FACE;
		//returnResults.osubtype = ECBPPOrientations::SOUTHFACE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.SouthFace);
	}

	// Top face check
	else if ((currentValueRef->y == dynamicGridCellBorders.corner_UpperNW.limit.y)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_LowerNW.limit.x)
		&&
		(currentValueRef->x < dynamicGridCellBorders.corner_LowerNE.limit.x)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_UpperNW.limit.z)
		&&
		(currentValueRef->z < dynamicGridCellBorders.corner_UpperSW.limit.z)
	)
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Top face found. " << std::endl;
		//returnResults.otype = ECBPPOrientations::FACE;
		//returnResults.osubtype = ECBPPOrientations::TOPFACE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.TopFace);
	}

	// Bottom Face check
	else if ((currentValueRef->y == minCalibrationGridLimit)
		&&
		(currentValueRef->x > dynamicGridCellBorders.corner_LowerNW.limit.x)
		&&
		(currentValueRef->x < dynamicGridCellBorders.corner_LowerNE.limit.x)
		&&
		(currentValueRef->z > dynamicGridCellBorders.corner_UpperNW.limit.z)
		&&
		(currentValueRef->z < dynamicGridCellBorders.corner_UpperSW.limit.z)
	)
	{
		//std::cout << "(PTriangleMeshPointCalibrator): Bottom face found. " << std::endl;
		//returnResults.otype = ECBPPOrientations::FACE;
		//returnResults.osubtype = ECBPPOrientations::BOTTOMFACE;
		applyCalibration(in_collisionPointRef, dynamicGridCellBorders.BottomFace);
	}

	else
	{
		//returnResults.otype = ECBPPOrientations::FREE;
		//returnResults.osubtype = ECBPPOrientations::FREE;
		//std::cout << "!!!! Warning !!!! point has NO FACE " << std::endl;
	}

}