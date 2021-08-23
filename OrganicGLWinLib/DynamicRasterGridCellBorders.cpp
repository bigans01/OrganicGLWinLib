#include "stdafx.h"
#include "DynamicRasterGridCellBorders.h"

void DynamicRasterGridCellBorders::setRasterGridCellBorders()
{
	// set up corner values ---------------------------------------------------------------------------------------

	// Lower NW coords
	corner_LowerNW.x = minBorder;
	corner_LowerNW.y = minBorder;
	corner_LowerNW.z = minBorder;

	// Lower SW coords
	corner_LowerSW.x = minBorder;
	corner_LowerSW.y = minBorder;
	corner_LowerSW.z = maxBorder;

	// Lower SE coords
	corner_LowerSE.x = maxBorder;
	corner_LowerSE.y = minBorder;
	corner_LowerSE.z = maxBorder;

	// Lower NE coords
	corner_LowerNE.x = maxBorder;
	corner_LowerNE.y = minBorder;
	corner_LowerNE.z = minBorder;

	// Upper NW coords
	corner_UpperNW.x = minBorder;
	corner_UpperNW.y = maxBorder;
	corner_UpperNW.z = minBorder;

	// Upper SW coords
	corner_UpperSW.x = minBorder;
	corner_UpperSW.y = maxBorder;
	corner_UpperSW.z = maxBorder;

	// Upper SE coords
	corner_UpperSE.x = maxBorder;
	corner_UpperSE.y = maxBorder;
	corner_UpperSE.z = maxBorder;

	// Upper NE coords
	corner_UpperNE.x = maxBorder;
	corner_UpperNE.y = maxBorder;
	corner_UpperNE.z = minBorder;

	// set up line values ---------------------------------------------------------------------------------------


	//start with x lines ------------------------------------------------------------------------------------


	// lower North
	

	// lower South


	// upper South

	// upper North


	// z lines ------------------------------------------------------------------------------------------------

	// lower West

	// upper West
	

	// upper East


	// lower East

	// y lines ------------------------------------------------------------------------------------------------

	// north East
	
	// south East

	// south West


	// north West
	



	// set up face values -----------------------------------------------------------------------------------------

	// west face


	// north face
	

	// east face 


	// south face


	// top face

	// bottom face
	
}