#include "stdafx.h"
#include "DynamicRasterGridCellBorders.h"

void DynamicRasterGridCellBorders::setRasterGridCellBorders()
{
	// set up corner values ---------------------------------------------------------------------------------------

	// Lower NW meta values
	corner_LowerNW.limit.x = minBorder;
	corner_LowerNW.limit.y = minBorder;
	corner_LowerNW.limit.z = minBorder;

	corner_LowerNW.applyXValue = 1;
	corner_LowerNW.applyYValue = 1;
	corner_LowerNW.applyZValue = 1;



	// Lower SW meta values
	corner_LowerSW.limit.x = minBorder;
	corner_LowerSW.limit.y = minBorder;
	corner_LowerSW.limit.z = maxBorder;

	corner_LowerSW.applyXValue = 1;
	corner_LowerSW.applyYValue = 1;
	corner_LowerSW.applyZValue = 1;


	// Lower SE meta values
	corner_LowerSE.limit.x = maxBorder;
	corner_LowerSE.limit.y = minBorder;
	corner_LowerSE.limit.z = maxBorder;

	corner_LowerSE.applyXValue = 1;
	corner_LowerSE.applyYValue = 1;
	corner_LowerSE.applyZValue = 1;


	// Lower NE meta values
	corner_LowerNE.limit.x = maxBorder;
	corner_LowerNE.limit.y = minBorder;
	corner_LowerNE.limit.z = minBorder;

	corner_LowerNE.applyXValue = 1;
	corner_LowerNE.applyYValue = 1;
	corner_LowerNE.applyZValue = 1;


	// Upper NW meta values
	corner_UpperNW.limit.x = minBorder;
	corner_UpperNW.limit.y = maxBorder;
	corner_UpperNW.limit.z = minBorder;

	corner_UpperNW.applyXValue = 1;
	corner_UpperNW.applyYValue = 1;
	corner_UpperNW.applyZValue = 1;


	// Upper SW meta values
	corner_UpperSW.limit.x = minBorder;
	corner_UpperSW.limit.y = maxBorder;
	corner_UpperSW.limit.z = maxBorder;

	corner_UpperSW.applyXValue = 1;
	corner_UpperSW.applyYValue = 1;
	corner_UpperSW.applyZValue = 1;

	// Upper SE meta values
	corner_UpperSE.limit.x = maxBorder;
	corner_UpperSE.limit.y = maxBorder;
	corner_UpperSE.limit.z = maxBorder;

	corner_UpperSE.applyXValue = 1;
	corner_UpperSE.applyYValue = 1;
	corner_UpperSE.applyZValue = 1;

	// Upper NE meta values
	corner_UpperNE.limit.x = maxBorder;
	corner_UpperNE.limit.y = maxBorder;
	corner_UpperNE.limit.z = minBorder;

	corner_UpperNE.applyXValue = 1;
	corner_UpperNE.applyYValue = 1;
	corner_UpperNE.applyZValue = 1;

	// set up line values ---------------------------------------------------------------------------------------


	//start with x lines ------------------------------------------------------------------------------------


	// lower North
	Xaxis_lowerNorth.limit.y = minBorder;
	Xaxis_lowerNorth.limit.z = minBorder;

	Xaxis_lowerNorth.applyYValue = 1;
	Xaxis_lowerNorth.applyZValue = 1;

	// lower South
	Xaxis_lowerSouth.limit.y = minBorder;
	Xaxis_lowerSouth.limit.z = maxBorder;

	Xaxis_lowerSouth.applyYValue = 1;
	Xaxis_lowerSouth.applyZValue = 1;


	// upper South
	Xaxis_upperSouth.limit.y = maxBorder;
	Xaxis_upperSouth.limit.z = maxBorder;

	Xaxis_upperSouth.applyYValue = 1;
	Xaxis_upperSouth.applyZValue = 1;


	// upper North
	Xaxis_upperNorth.limit.y = maxBorder;
	Xaxis_upperNorth.limit.z = minBorder;

	Xaxis_upperNorth.applyYValue = 1;
	Xaxis_upperNorth.applyZValue = 1;


	// z lines ------------------------------------------------------------------------------------------------

	// lower West
	Zaxis_lowerWest.limit.x = minBorder;
	Zaxis_lowerWest.limit.y = minBorder;

	Zaxis_lowerWest.applyXValue = 1;
	Zaxis_lowerWest.applyYValue = 1;

	// upper West
	Zaxis_upperWest.limit.x = minBorder;
	Zaxis_upperWest.limit.y = maxBorder;

	Zaxis_upperWest.applyXValue = 1;
	Zaxis_upperWest.applyYValue = 1;
	

	// upper East
	Zaxis_upperEast.limit.x = maxBorder;
	Zaxis_upperEast.limit.y = maxBorder;

	Zaxis_upperEast.applyXValue = 1;
	Zaxis_upperEast.applyYValue = 1;


	// lower East
	Zaxis_lowerEast.limit.x = maxBorder;
	Zaxis_lowerEast.limit.y = minBorder;

	Zaxis_lowerEast.applyXValue = 1;
	Zaxis_lowerEast.applyYValue = 1;

	// y lines ------------------------------------------------------------------------------------------------

	// north East
	Yaxis_northEast.limit.x = maxBorder;
	Yaxis_northEast.limit.z = minBorder;

	Yaxis_northEast.applyXValue = 1;
	Yaxis_northEast.applyZValue = 1;
	
	// south East
	Yaxis_southEast.limit.x = maxBorder;
	Yaxis_southEast.limit.z = maxBorder;

	Yaxis_southEast.applyXValue = 1;
	Yaxis_southEast.applyZValue = 1;

	// south West
	Yaxis_southWest.limit.x = minBorder;
	Yaxis_southWest.limit.z = maxBorder;

	Yaxis_southWest.applyXValue = 1;
	Yaxis_southWest.applyZValue = 1;


	// north West
	Yaxis_northWest.limit.x = minBorder;
	Yaxis_northWest.limit.z = minBorder;

	Yaxis_northWest.applyXValue = 1;
	Yaxis_northWest.applyZValue = 1;



	// set up face values -----------------------------------------------------------------------------------------

	// west face
	WestFace.limit.x = minBorder;
	WestFace.applyXValue = 1;

	// north face
	NorthFace.limit.z = minBorder;
	NorthFace.applyZValue = 1;
	
	// east face 
	EastFace.limit.x = maxBorder;
	EastFace.applyXValue = 1;

	// south face
	SouthFace.limit.z = maxBorder;
	SouthFace.applyZValue = 1;

	// top face
	TopFace.limit.y = maxBorder;
	TopFace.applyYValue = 1;

	// bottom face
	BottomFace.limit.y = minBorder;
	BottomFace.applyYValue = 1;
	
}