#include "stdafx.h"
#include "RasterCubeTracerBase.h"

void RasterCubeTracerBase::iterateToNextBlock()
{
	float x_interceptCoord = 0.0f;
	float y_interceptCoord = 0.0f;
	float z_interceptCoord = 0.0f;

	float time_to_complete_x_traversal = 0.0f;
	float time_to_complete_y_traversal = 0.0f;
	float time_to_complete_z_traversal = 0.0f;

	ECBPolyPoint calculatedPoint_for_x;
	ECBPolyPoint calculatedPoint_for_y;
	ECBPolyPoint calculatedPoint_for_z;

	ECBPolyPoint currentIterationBeginPoint(currentTracingPoint.x, currentTracingPoint.y, currentTracingPoint.z);

	// ------------------------------- get X values
	if (normalizedSlopeDirection.x > 0)
	{
		x_interceptCoord = rasterCubeDimLength;
		float origin_to_border_x_diff = (x_interceptCoord - currentIterationBeginPoint.x);
		time_to_complete_x_traversal = origin_to_border_x_diff / tracerDirectionVector.x;

		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (tracerDirectionVector.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_x_traversal);

		if (debugFlag == true)
		{
			std::cout << "---->: x_interceptCoord: " << x_interceptCoord << std::endl;
			std::cout << "---->: currentIterationBeginPoint: " << currentIterationBeginPoint.x << ", " << currentIterationBeginPoint.y << ", " << currentIterationBeginPoint.z << std::endl;
			std::cout << "---->: origin_to_border_x_diff: " << origin_to_border_x_diff << std::endl;
			std::cout << "---->: tracer direction vector x: " << tracerDirectionVector.x << std::endl;
			std::cout << "---->: time to complete traversal: " << time_to_complete_x_traversal << std::endl;
		}
	}
	else if (normalizedSlopeDirection.x < 0)
	{
		x_interceptCoord = 0.0f;
		float origin_to_border_x_diff = abs(x_interceptCoord - currentIterationBeginPoint.x);					// make sure to get absolute value for these two lines (WEST border)
		time_to_complete_x_traversal = abs(origin_to_border_x_diff / tracerDirectionVector.x);	// ""

		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (tracerDirectionVector.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_x_traversal);

		if (debugFlag == true)
		{

		}
	}

	// """ y values
	if (normalizedSlopeDirection.y > 0)
	{
		y_interceptCoord = rasterCubeDimLength;
		float origin_to_border_y_diff = (y_interceptCoord - currentIterationBeginPoint.y);
		time_to_complete_y_traversal = origin_to_border_y_diff / tracerDirectionVector.y;

		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (tracerDirectionVector.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_y_traversal);

		if (debugFlag == true)
		{

		}
	}
	else if (normalizedSlopeDirection.y < 0)
	{
		y_interceptCoord = 0.0f;
		float origin_to_border_y_diff = abs(y_interceptCoord - currentIterationBeginPoint.y);
		time_to_complete_y_traversal = abs(origin_to_border_y_diff / tracerDirectionVector.y);

		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (tracerDirectionVector.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_y_traversal);

		if (debugFlag == true)
		{

		}
	}

	// """ z values
	if (normalizedSlopeDirection.z > 0)
	{
		z_interceptCoord = rasterCubeDimLength;
		float origin_to_border_z_diff = (z_interceptCoord - currentIterationBeginPoint.z);
		time_to_complete_z_traversal = origin_to_border_z_diff / tracerDirectionVector.z;

		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (tracerDirectionVector.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (tracerDirectionVector.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		if (debugFlag == true)
		{

		}
	}
	else if (normalizedSlopeDirection.z < 0)
	{
		z_interceptCoord = 0.0f;
		float origin_to_border_z_diff = abs(z_interceptCoord - currentIterationBeginPoint.z);
		time_to_complete_z_traversal = abs(origin_to_border_z_diff / tracerDirectionVector.z);

		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (tracerDirectionVector.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (tracerDirectionVector.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;

		if (debugFlag == true)
		{

		}
	}

	ECBPolyPointTri triPointParam;
	triPointParam.triPoints[0] = calculatedPoint_for_x;
	triPointParam.triPoints[1] = calculatedPoint_for_y;
	triPointParam.triPoints[2] = calculatedPoint_for_z;
	ECBPolyPoint distanceValues;
	distanceValues.x = time_to_complete_x_traversal;
	distanceValues.y = time_to_complete_y_traversal;
	distanceValues.z = time_to_complete_z_traversal;

	if (debugFlag == true)
	{
		std::cout << "Time_to_complete_x_traversal: " << time_to_complete_x_traversal << std::endl;
		std::cout << "Distance values: " << distanceValues.x << ", " << distanceValues.y << ", " << distanceValues.z << std::endl;
	}

	DynamicEndpointMeta dynamicPointMeta = getDynamicEndpointMetaData(currentIterationBeginPoint, &dynamicBorder, distanceValues, normalizedSlopeDirection, triPointParam);

	glm::vec3 oldTracingPoint = currentTracingPoint;

	glm::vec3 distOriginPoint = currentTracingPoint;
	glm::vec3 distTravelledToPoint(dynamicPointMeta.pointLocation.x, dynamicPointMeta.pointLocation.y, dynamicPointMeta.pointLocation.z);
	float traveledDistance = glm::distance(distOriginPoint, distTravelledToPoint);
	remainingDistance -= traveledDistance;

	currentTracingPoint.x = dynamicPointMeta.pointLocation.x;
	currentTracingPoint.y = dynamicPointMeta.pointLocation.y;
	currentTracingPoint.z = dynamicPointMeta.pointLocation.z;

	if (dynamicPointMeta.dimensionResetValues.x != -1 && (normalizedSlopeDirection.x != 0.0f))
	{
		currentTracingPoint.x = dynamicPointMeta.dimensionResetValues.x;
	}
	if (dynamicPointMeta.dimensionResetValues.y != -1 && (normalizedSlopeDirection.y != 0.0f))
	{
		currentTracingPoint.y = dynamicPointMeta.dimensionResetValues.y;
	}
	if (dynamicPointMeta.dimensionResetValues.z != -1 && (normalizedSlopeDirection.z != 0.0f))
	{
		currentTracingPoint.z = dynamicPointMeta.dimensionResetValues.z;
	}

	currentCubeKey.x += dynamicPointMeta.moveDirectionValues.x;
	currentCubeKey.y += dynamicPointMeta.moveDirectionValues.y;
	currentCubeKey.z += dynamicPointMeta.moveDirectionValues.z;

	//std::cout << "current key is now: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;

	//if (debugFlag == true)
	//{
		if (dynamicPointMeta.moveDirectionValues.isAllZero() == true)
		{
			std::cout << "!!! WARNING: is all zero detected..." << std::endl;
			std::cout << "|||||||||| :::: iterated to next block, stats are: " << std::endl;
			std::cout << "::::: traveled distance value: " << traveledDistance << std::endl;
			std::cout << "old traced point: " << oldTracingPoint.x << ", " << oldTracingPoint.y << ", " << oldTracingPoint.z << std::endl;
			std::cout << "traced endpoint: " << dynamicPointMeta.pointLocation.x << ", " << dynamicPointMeta.pointLocation.y << ", " << dynamicPointMeta.pointLocation.z << std::endl;
			std::cout << "value of currentTracingPoint: " << currentTracingPoint.x << ", " << currentTracingPoint.y << ", " << currentTracingPoint.z << std::endl;
			std::cout << "resulting moveDirectionValues: " << dynamicPointMeta.moveDirectionValues.x << ", " << dynamicPointMeta.moveDirectionValues.y << ", " << dynamicPointMeta.moveDirectionValues.z << std::endl;

			int waitVal = 3;
			std::cin >> waitVal;
		}
	//}
	//std::cout << "new cube key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;

}

void RasterCubeTracerBase::setData(EnclaveKeyDef::EnclaveKey in_startCubeKey,
	EnclaveKeyDef::EnclaveKey in_endCubeKey,
	glm::vec3 in_startCubePoint,
	glm::vec3 in_endCubePoint,
	float in_rasterCubeDimLength,
	float in_tileWeightRatio,
	bool in_debugFlag)
{
	debugFlag = in_debugFlag;
	rasterCubeDimLength = in_rasterCubeDimLength;
	dynamicBorder.constructBorders(rasterCubeDimLength);
	tileWeightRatio = in_tileWeightRatio;

	//std::cout << "start key: " << in_startCubeKey.x << ", " << in_startCubeKey.y << ", " << in_startCubeKey.z << std::endl;
	//std::cout << "end key: " << in_endCubeKey.x << ", " << in_endCubeKey.y << ", " << in_endCubeKey.z << std::endl;

	setUpNextRun(in_startCubeKey, in_endCubeKey, in_startCubePoint, in_endCubePoint);

	/*
	std::cout << "(After setup) start key: " << startCubeKey.x << ", " << startCubeKey.y << ", " << startCubeKey.z << std::endl;
	std::cout << "(After setup) current cube key: " << currentCubeKey.x << ", " << currentCubeKey.y << ", " << currentCubeKey.z << std::endl;
	std::cout << "(After setup) end key: " << endCubeKey.x << ", " << endCubeKey.y << ", " << endCubeKey.z << std::endl;
	std::cout << "(After setup) ---> startCubePoint: " << startCubePoint.x << ", " << startCubePoint.y << ", " << startCubePoint.z << std::endl;
	std::cout << "(After setup) ---> endCubePoint: " << endCubePoint.x << ", " << endCubePoint.y << ", " << endCubePoint.z << std::endl;
	std::cout << "(After setup) ---> tracerDirectionVector: " << tracerDirectionVector.x << ", " << tracerDirectionVector.y << ", " << tracerDirectionVector.z << std::endl;
	*/
}

void RasterCubeTracerBase::setUpNextRun(EnclaveKeyDef::EnclaveKey in_startCubeKey,
	EnclaveKeyDef::EnclaveKey in_endCubeKey,
	glm::vec3 in_startCubePoint,
	glm::vec3 in_endCubePoint)
{
	startCubeKey = in_startCubeKey;
	currentCubeKey = in_startCubeKey;
	endCubeKey = in_endCubeKey;
	startCubePoint = in_startCubePoint;
	endCubePoint = in_endCubePoint;

	// the beginning tracing point is always equal to the center of a cube, which is equal to a point having half the value 
	// for rasterCubeDimLength for x, y and z directions.
	glm::vec3 tracingPoint(rasterCubeDimLength, rasterCubeDimLength, rasterCubeDimLength);
	currentTracingPoint = tracingPoint;
	currentTracingPoint /= 2;

	tracerDirectionVector = endCubePoint - startCubePoint;
	ECBPolyPoint convertedDirectionVector(tracerDirectionVector.x, tracerDirectionVector.y, tracerDirectionVector.z);
	normalizedSlopeDirection = IndependentUtils::findNormalizedPoint(convertedDirectionVector);

	//lineLength = glm::distance(startCubePoint, endCubePoint) / 100;
	lineLength = glm::distance(startCubePoint, endCubePoint);
	remainingDistance = lineLength;
}

void RasterCubeTracerBase::setOptionalCubeLookupRef(RasterCubeLookup* in_optionalCubeLookupRef)
{
	optionalCubeLookup = in_optionalCubeLookupRef;
}

void RasterCubeTracerBase::setOptionalPolyLoggerRef(PolyLogger* in_optionalLoggerRef)
{
	optionalLoggerRef = in_optionalLoggerRef;
}

void RasterCubeTracerBase::setOptionalMaterialID(short in_materialID)
{
	optionalTracerMaterialID = in_materialID;
}

DynamicEndpointMeta RasterCubeTracerBase::getDynamicEndpointMetaData(ECBPolyPoint in_originPoint,
	DynamicBorderLineList* in_blockBorderRef,
	ECBPolyPoint in_distanceValues,
	ECBPolyPoint in_slopeDirection,
	ECBPolyPointTri in_XYZinterceptCoords)
{
	StringTracer endpointTracer;
	DynamicEndpointMeta calculatedEndpointData;
	int isPointOnALine = 1;	// 0 indicates a condition in which xyz distance are all equal, 1 indicates two other axis are equal (point would be on a line in that case)
													//float dist_to_X = in_distanceValues.x;			// get distance for x
													//float dist_to_Y = in_distanceValues.y;			// ... for y
													//float dist_to_Z = in_distanceValues.z;			// ... for z

	//std::cout << "Original dist values: " << in_distanceValues.x << ", " << in_distanceValues.y << ", " << in_distanceValues.z << std::endl;

	ECBPolyPoint calibratedDistances = roundXYZInterceptDistancesToAppropriatePrecision(in_distanceValues.x, in_distanceValues.y, in_distanceValues.z);
	float dist_to_X = calibratedDistances.x;
	float dist_to_Y = calibratedDistances.y;
	float dist_to_Z = calibratedDistances.z;

	endpointTracer.insertTracingString(std::string("dist_to_X: " + std::to_string(dist_to_X)));
	endpointTracer.insertTracingString(std::string("dist_to_Y: " + std::to_string(dist_to_Y)));
	endpointTracer.insertTracingString(std::string("dist_to_Z: " + std::to_string(dist_to_Z)));


	ECBPolyPoint x_intercept_coords = in_XYZinterceptCoords.triPoints[0];	// get x point																										
	ECBPolyPoint y_intercept_coords = in_XYZinterceptCoords.triPoints[1];	// ...y point																									
	ECBPolyPoint z_intercept_coords = in_XYZinterceptCoords.triPoints[2];	// ...z point

	endpointTracer.insertTracingString(std::string("x_intercept_coord: " + std::to_string(in_XYZinterceptCoords.triPoints[0].x) + ", "
		+ std::to_string(in_XYZinterceptCoords.triPoints[0].y) + ", " + std::to_string(in_XYZinterceptCoords.triPoints[0].z )+ "\n"));

	endpointTracer.insertTracingString(std::string("y_intercept_coord: " + std::to_string(in_XYZinterceptCoords.triPoints[1].x) + ", "
		+ std::to_string(in_XYZinterceptCoords.triPoints[1].y) + ", " + std::to_string(in_XYZinterceptCoords.triPoints[1].z) + "\n"));

	endpointTracer.insertTracingString(std::string("z_intercept_coord: " + std::to_string(in_XYZinterceptCoords.triPoints[2].x) + ", "
		+ std::to_string(in_XYZinterceptCoords.triPoints[2].y) + ", " + std::to_string(in_XYZinterceptCoords.triPoints[2].z) + "\n"));

	if (debugFlag == true)
	{
		//std::cout << "Original distance values: " << calibratedDistances.x << ", " << calibratedDistances.y << ", " << calibratedDistances.z << std::endl;
		//std::cout << "Dim distance values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;
		//std::cout << "::::::::: X-intercept coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << std::endl;
		//std::cout << "::::::::: Y-intercept coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << std::endl;
		//std::cout << "::::::::: Z-intercept coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;
	}
	//std::cout << "Original distance values: " << calibratedDistances.x << ", " << calibratedDistances.y << ", " << calibratedDistances.z << std::endl;
	//std::cout << "Dim distance values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;
	//std::cout << "::::::::: X-intercept coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << std::endl;
	//std::cout << "::::::::: Y-intercept coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << std::endl;
	//std::cout << "::::::::: Z-intercept coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << std::endl;

	// We must determine if any distances are equal: It is a corner when:
	// A: X/Y/Z distances are equal
	// B: Two distances are equal,  and the remaining distance's respective coordinate is either on 1.0f or 0.0f.

	// OLD LOGIC

	/*
	if
		(
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
			||																// remove this and folliwng 5 lines for debug
			((dist_to_X == dist_to_Y) && dist_to_Z == 0.0f)
			||
			((dist_to_Y == dist_to_Z) && dist_to_X == 0.0f)
			||
			((dist_to_X == dist_to_Z) && dist_to_Y == 0.0f)

		)
	*/


	if
		(
		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))
			||																// remove this and folliwng 5 lines for debug
			(
			(dist_to_X == dist_to_Y) &&
				(dist_to_Z == 0.0f) &&
				(
				(in_originPoint.z == rasterCubeDimLength)
					||
					(in_originPoint.z == 0.0f)
					)
				)
			||
			(
			(dist_to_Y == dist_to_Z) &&
				(dist_to_X == 0.0f) &&
				(
				(in_originPoint.x == rasterCubeDimLength)
					||
					(in_originPoint.x == 0.0f)
					)
				)
			||
			(
			(dist_to_X == dist_to_Z) &&
				(dist_to_Y == 0.0f) &&
				(
				(in_originPoint.y == rasterCubeDimLength)
					||
					(in_originPoint.y == 0.0f)
					)
				)

			)

	{
		//std::cout << "ALL distances equal! " << std::endl;
		isPointOnALine = 0;								// corner type mode to 0
	}
	ECBPolyPoint pointToCheck;				// the actual intercept point that will be used when comparing to border lines, border corners, or border faces
											// Special condition: check to see if origin point is directly on one of the x/y/z planes
											//if (dist_to_Y == dist_to_Z)
											//{
											//std::cout << "Y and Z dist is equal!" << std::endl;
											//}

											// Step 1: check if the resulting point is a perfect corner intercept
											// Multiple conditions:
											//		Condition 1: X, Y, -AND- Z intercept distance are the same 
											//		Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
											//      Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
											//		Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
											//      Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)
											//      Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
											//      Condition 7: in_slope.z is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (z intercept coord's x and y are exactly on border)
	//std::cout << "Origin point: " << in_originPoint.x << ", " << in_originPoint.y << ", " << in_originPoint.z << std::endl;
	//std::cout << "Dist values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << " |||| Slopes: " << in_slopeDirection.x << ", " << in_slopeDirection.y << ", " << in_slopeDirection.z << std::endl;
	//std::cout << "Hundred thousandth rounding vals are: " << new_x << ", " << new_y << ", " << new_z << std::endl;
	//std::cout << "---------Calculated point MetaData for this iteration-----------" << std::endl;
	//std::cout << "testyval: " << testyval << std::endl;
	//std::cout << "X-intercept coords: " << x_intercept_coords.x << ", " << x_intercept_coords.y << ", " << x_intercept_coords.z << " | Distance to x-intercept: " << dist_to_X << std::endl;
	//std::cout << "Y-intercept coords: " << y_intercept_coords.x << ", " << y_intercept_coords.y << ", " << y_intercept_coords.z << " | Distance to y-intercept: " << dist_to_Y << std::endl;
	//std::cout << "Z-intercept coords: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << " | Distance to z-intercept: " << dist_to_Z << std::endl;

	//std::cout << "Distance values: " << dist_to_X << ", " << dist_to_Y << ", " << dist_to_Z << std::endl;


	if (

		((dist_to_X == dist_to_Y) && (dist_to_Y == dist_to_Z))	// Condition 1: X, Y, -AND - Z intercept distance are the same

		||

		(														//	Condition 2: dist_to_X equals dist_to_Y -AND- the z value for the origin point is at the north or the south face -AND- the z slope is 0
		(dist_to_X == dist_to_Y)
			&&
			((in_originPoint.z == rasterCubeDimLength) || (in_originPoint.z == 0.0f))
			&&
			in_slopeDirection.z == 0.0f
			)

		||

		(														//	Condition 3: dist_to_X equals dist_to_Z -AND- the y value for the origin point is at the top or bottom face -AND- the y slope is 0
		(dist_to_X == dist_to_Z)
			&&
			((in_originPoint.y == rasterCubeDimLength) || (in_originPoint.y == 0.0f))
			&&
			in_slopeDirection.y == 0.0f
			)

		||

		(														//	Condition 4: dist_to_Y equals dist_to_Z -AND- the x value for the origin point is at the east or west face -AND- the x slope is 0
		(dist_to_Y == dist_to_Z)
			&&
			((in_originPoint.x == rasterCubeDimLength) || (in_originPoint.x == 0.0f))
			&&
			in_slopeDirection.x == 0.0f
			)

		||

		(														//  Condition 5: in_slope.x is not 0 -AND- in_slope.y is 0 -AND- in_slope.z is 0 -AND (x intercept coord's y and z are exactly on border)		
		(dist_to_X != 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((x_intercept_coords.y == 1.0f) || (x_intercept_coords.y == 0.0f))
				&&
				((x_intercept_coords.z == 1.0f) || (x_intercept_coords.z == 0.0f))
				)
			)

		||

		(														//   Condition 6: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Y != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.z == 0.0f)
			&&
			(
			((y_intercept_coords.x == 1.0f) || (y_intercept_coords.x == 0.0f))
				&&
				((y_intercept_coords.z == 1.0f) || (y_intercept_coords.z == 0.0f))
				)
			)

		||

		(														//   Condition 7: in_slope.y is not 0 -AND- in_slope.x is 0 -AND- in_slope.z is 0 -AND (y intercept coord's x and z are exactly on border)
		(dist_to_Z != 0.0f)
			&&
			(in_slopeDirection.x == 0.0f)
			&&
			(in_slopeDirection.y == 0.0f)
			&&
			(
			((z_intercept_coords.x == 1.0f) || (z_intercept_coords.x == 0.0f))
				&&
				((z_intercept_coords.y == 1.0f) || (z_intercept_coords.y == 0.0f))
				)
			)


		)

	{
		if (debugFlag == true)
		{
			std::cout << "++++++++++++++++++++++++++++++++++++++++ entered CORNER logic if statement..." << std::endl;
		}
		//std::cout << ":::: corner point hit! " << std::endl;
		/**/

		endpointTracer.insertTracingString(std::string("Tracer entered CORNER branch."));

		if (in_slopeDirection.x != 0.0f)
		{
			//pointToCheck = roundToNearestDynamicLineOrCorner(0, x_intercept_coords, isPointOnALine);		// x_intercept coords
			pointToCheck = bindToNearestCorner(0, x_intercept_coords);
																											//std::cout << "point to check is x: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
		}
		else if (in_slopeDirection.y != 0.0f)
		{
			//pointToCheck = roundToNearestDynamicLineOrCorner(1, y_intercept_coords, isPointOnALine);		// y_intercept_coords
																											//std::cout << "point to check is y: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
			pointToCheck = bindToNearestCorner(1, y_intercept_coords);
		}
		else if (in_slopeDirection.z != 0.0f)
		{
			//pointToCheck = roundToNearestDynamicLineOrCorner(2, z_intercept_coords, isPointOnALine);		// z_intercept coords
																											//std::cout << "point to check is z: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
			pointToCheck = bindToNearestCorner(2, z_intercept_coords);
		}

		//std::cout << "corner intersection detected!!!" << std::endl;
		if (pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x	&&		pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y	&&		pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)		// Lower NW
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerNW.borderValues);
			//std::cout << "Point is at lower NW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
			//std::cout << "endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerSW.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_LowerSW.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)			// Lower SW
		{
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerSW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SW
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerSW.borderValues);
			//std::cout << "Point is at lower SW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
			//std::cout << "endpoint is: " << calculatedEndpointData.pointLocation.x << ", " << calculatedEndpointData.pointLocation.y << ", " << calculatedEndpointData.pointLocation.z << ", " << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperSW.pointXYZ.x   &&	pointToCheck.y == in_blockBorderRef->corner_UpperSW.pointXYZ.y   &&  pointToCheck.z == in_blockBorderRef->corner_UpperSW.pointXYZ.z)			// Upper SW	 
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperSW.borderValues);
			//std::cout << "Point is at upper SW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperNW.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperNW.pointXYZ.z)			// Upper NW
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperNW.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperNW.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NW
			//std::cout << "Point is at upper NW...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x	&&  pointToCheck.y == in_blockBorderRef->corner_LowerNE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerNE.pointXYZ.z)			// Lower NE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerNE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower NE
			//std::cout << "Point is at lower NE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_LowerSE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_LowerSE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_LowerSE.pointXYZ.z)			// Lower SE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_LowerSE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_LowerSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, lower SE
			//std::cout << "Point is at lower SE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperSE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperSE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperSE.pointXYZ.z)			// Upper SE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperSE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperSE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper SE
			//std::cout << "Point is at upper SE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
		else if (pointToCheck.x == in_blockBorderRef->corner_UpperNE.pointXYZ.x	&&	pointToCheck.y == in_blockBorderRef->corner_UpperNE.pointXYZ.y	&&	pointToCheck.z == in_blockBorderRef->corner_UpperNE.pointXYZ.z)			// Upper NE
		{
			calculatedEndpointData.applyBlockBorder(in_blockBorderRef->corner_UpperNE.borderValues);
			//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->corner_UpperNE.cornerAmpValues, in_originPoint, pointToCheck);	// get the shifting key, upper NE
			//std::cout << "Point is at upper NE...move values are: " << calculatedEndpointData.moveDirectionValues.x << ", " << calculatedEndpointData.moveDirectionValues.y << ", " << calculatedEndpointData.moveDirectionValues.z << std::endl;
		}
	}

	// Step 2: check if the resulting point is a border line intercept
	else if ((dist_to_X == dist_to_Y) || (dist_to_Y == dist_to_Z) || (dist_to_X == dist_to_Z))
	{
		endpointTracer.insertTracingString(std::string("Tracer entered LINE branch."));
		//std::cout << "++++++++++++++++++++++++++++++++++++++++ entered LINE logic if statement..." << std::endl;
		if (debugFlag == true)
		{
			//std::cout << "++++++++++++++++++++++++++++++++++++++++ entered LINE logic if statement..." << std::endl;
		}
		//std::cout << ">>>>> PHASE 2 entry: " << std::endl;
		// ||||||||||||||||||||||||||||||||||||||||| condition 1 (Z-lines): check for Z line intercepts, when X and Y are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		if ((dist_to_X == dist_to_Y) && (dist_to_X != 0.0f))							// condition 1: X equals Y, but neither X or Y are 0 (0 would indicate their distances are not used)	
		{
			//********DO NOT put pointToCheck before the if statements below; this is because of the updated logic for 
			// DynamicLinePointSynchronizer on 6/13/2021
			endpointTracer.insertTracingString(std::string("Tracer entered Z-Line intercept branch."));

			// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// Check if Z distance, which is not one of the twins of X/Y, is greater than X or Y...if it is, X and Y hit first, and they are on a line.
			// Either the X-intercept coord or Y-intercept coord may be used in the call to bindNearestToLine.
			if (
				(dist_to_Z > dist_to_X) && (in_slopeDirection.z != 0.0f)		//     condition 1: if Z's distance is greater  than X (could also be y here), AND it's slope is not 0, we go with x/y distance point
				||																//		-- OR --
				(in_slopeDirection.z == 0.0f)									//     condition 2: Z's slope is 0, check x and y
				)	// we can compare to either x or y, since they are both the same value; if Z is greater, it will not hit a Z-face before X and Y does
			{
				//std::cout << "X equals Y, Non-face entry" << std::endl;
				// use x_intercept coords

				//pointToCheck = roundToNearestDynamicLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, line or corner type
				pointToCheck = bindToNearestLine(0, x_intercept_coords, DynamicLinePointSyncExclusionFlags::EXCLUDE_Z);
				//perform comparisons to Zaxis borders -- lowerWest, lowerEast, upperWest, upperEast
				// Lower West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_LowerNW.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_LowerSW.pointXYZ.z)			// is z between the LowerNW and LowerSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_lowerWest);
					//std::cout << " ||||||>>>>>>>>>>test if hit " << std::endl;
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_lowerWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower west line ...shift key is: " << std::endl;
					//std::cout << "Point is at lower west line ...shift key is: " << std::endl;
				}

				// Upper West line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_UpperNW.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_UpperNW.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_UpperSW.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_upperWest);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_upperWest, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at upper west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at upper west line ...shift key is: " << std::endl;
				}

				// Upper East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_UpperNE.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_UpperNE.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_UpperNE.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_UpperSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_upperEast);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_upperEast, in_originPoint, pointToCheck);	// get the shifting key				
					//std::cout << "Point is at upper east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at upper east line ...shift key is: " << std::endl;
				}
				// Lower East line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x)
					&&
					(pointToCheck.y == in_blockBorderRef->corner_LowerNE.pointXYZ.y)
					&&
					(pointToCheck.z > in_blockBorderRef->corner_LowerNE.pointXYZ.z	&& pointToCheck.z < in_blockBorderRef->corner_LowerSE.pointXYZ.z)			// is z between the UpperNW and UpperSW corners, but not equal to either of them?
					)
				{
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Zaxis_lowerEast);
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Zaxis_lowerEast, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << "Point is at lower east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at lower east line ...shift key is: " << std::endl;
				}

			}

			// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// otherwise, the non-twin of Z is the closest intercept. So just use it's coords, and don't round any points to line or corner.
			else if ((dist_to_Z < dist_to_X) && (in_slopeDirection.z != 0.0f))	// Z-face gets intersected before X/Y, so it's a face hit and we don't need to edit the points.

			{
				std::cout << "X equals Y, Face entry" << std::endl;
				pointToCheck = z_intercept_coords;	// make pointToCheck equal to z coord, since it hits a z face before x/y
				if (in_slopeDirection.z == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->SouthFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
				}
				else if (in_slopeDirection.z == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->NorthFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
				}

			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 2 (Y-lines): check for Y line intercepts, , when X and Z are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X == dist_to_Z) && (dist_to_X != 0.0f))							// condition 1: X equals Z, but neither X or Z are 0 (0 would indicate their distance is not used)	
		{
			endpointTracer.insertTracingString(std::string("Tracer entered Y-Line intercept branch."));
			if (debugFlag == true)
			{
				//std::cout << "++++++++++++++++++++++++++++++++++++++++ entered LINE > : dist_to_X ==  dist_to_Z : " << std::endl;
			}
			//std::cout << "++++++++++++++++++++++++++++++++++++++++ entered LINE > : dist_to_X ==  dist_to_Z : " << std::endl;

			//********DO NOT put pointToCheck before the if statements below; this is because of the updated logic for 
			// DynamicLinePointSynchronizer on 6/13/2021

			// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// check if Y distance, which is not one of the twins of X/Z, is greater than X or Z...if it is, X and Z hit first, and they are on a line.
			// Either the X-intercept coord or Z-intercept coord may be used in the call to bindNearestToLine.
			if (
				(dist_to_Y > dist_to_X) && (in_slopeDirection.y != 0.0f)			// condition 1:	if Y's distance is greater  than X (could also be z here), AND it's slope is not 0, we go with x/z distance point
				||
				(in_slopeDirection.y == 0.0f)										// condition 2: Y's slope is 0, check x and y	
				)
			{
				//pointToCheck = roundToNearestDynamicLineOrCorner(0, x_intercept_coords, isPointOnALine);	// parameters: 0 = round other points to x, use x-intercept coords, corner type			
				pointToCheck = bindToNearestLine(0, x_intercept_coords, DynamicLinePointSyncExclusionFlags::EXCLUDE_Y);
				if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNE.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerNE.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperNE.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_northEast, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_northEast);
					//std::cout << "Point is at north east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at north east line. " << std::endl;
				}

				// South East line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerSE.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSE.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerSE.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperSE.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_southEast, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_southEast);
					//std::cout << "Point is at south east line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at south east line. " << std::endl;
				}
				// South West line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerSW.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerSW.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperSW.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_southWest, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_southWest);
					//std::cout << "Point is at south west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at south west line. " << std::endl;
				}
				// North West line
				else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)
					&&
					(pointToCheck.y > in_blockBorderRef->corner_LowerNW.pointXYZ.y	&& pointToCheck.y < in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Yaxis_northWest, in_originPoint, pointToCheck);	// get the shifting key
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Yaxis_northWest);
					//std::cout << "Point is at north west line ...shift key is: " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
					//std::cout << "Point is at north west line. " << std::endl;
				}
				else
				{
					//std::cout << "!!!!!!!!!!!!!!!!!!!! +++++++ WARNING, no line matched in this branch. " << std::endl;
				}


			}

			// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// otherwise, the non-twin of Y is the closest intercept. So just use it's coords, and don't round any points to line or corner.
			else if ((dist_to_Y < dist_to_X) && (in_slopeDirection.y != 0.0f))
			{
				pointToCheck = y_intercept_coords;
				if (in_slopeDirection.y == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->TopFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
					//std::cout << "X/Z distances match, but Y is hit first (positive y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
				else if (in_slopeDirection.y == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->BottomFace, in_originPoint, pointToCheck);
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
					//std::cout << "X/Z distances match, but Y is hit first (negative y): " << newKey.x << ", " << newKey.y << ", " << newKey.z << std::endl;
				}
			}
		}
		// ||||||||||||||||||||||||||||||||||||||||| condition 3 (X-lines): Y and Z distances match, when Y and Z are equal but neither are 0 ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y == dist_to_Z) && (dist_to_Y != 0.0f))
		{
			endpointTracer.insertTracingString(std::string("Tracer entered X-Line intercept branch."));
			//std::cout << "BRANCH HIT: DIST_TO_Y equals DIST_TO_Z" << std::endl;
			//std::cout << "Slope direction x: " << in_slopeDirection.x << std::endl;
			//std::cout << "is point on a line>: " << isPointOnALine << std::endl;
			//std::cout << "Z-int coourds, before modification: " << z_intercept_coords.x << ", " << z_intercept_coords.y << ", " << z_intercept_coords.z << " | Distance to z-intercept: " << dist_to_Z << std::endl;
			//pointToCheck = roundToNearestDynamicLineOrCorner(2, z_intercept_coords, isPointOnALine);

			//********DO NOT put pointToCheck before the if statements below; this is because of the updated logic for 
			// DynamicLinePointSynchronizer on 6/13/2021

			// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// check if X distance, which is not one of the twins of Y/Z, is greater than Y or Z...if it is, Y and Z hit first, and they are on a line.
			// Either the Y-intercept coord or Z-intercept coord may be used in the call to bindNearestToLine.
			if (
				(dist_to_X > dist_to_Y) && (in_slopeDirection.x != 0.0f)	// condition 1:	if X's distance is greater  than Y (could also be Z here), AND it's slope is not 0, we go with y/z distance point
				||															// --OR-- 
				(in_slopeDirection.x == 0.0f)								// condition 2: X's slope is 0, check y and z
				)
			{
				//pointToCheck = roundToNearestDynamicLineOrCorner(2, z_intercept_coords, isPointOnALine);
				pointToCheck = bindToNearestLine(2, z_intercept_coords, DynamicLinePointSyncExclusionFlags::EXCLUDE_X);
				//std::cout << " >>>> Y equals Z (branch 1) " << std::endl;
				//std::cout << " >>>> Point to check values: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
				if ((pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_LowerNW.pointXYZ.x	&&		pointToCheck.x < in_blockBorderRef->corner_LowerNE.pointXYZ.x)	// is x between the LowerNW and LowerNE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_lowerNorth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 1) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_lowerNorth);
				}
				// Lower South  line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_LowerSW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_LowerSW.pointXYZ.x	&&	pointToCheck.x < in_blockBorderRef->corner_LowerSE.pointXYZ.x)		// is x between the LowerSW and LowerSE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_lowerSouth, in_originPoint, pointToCheck);	// get the shifting key

					//std::cout << " >>>> Y equals Z (branch 1, 2) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_lowerSouth);
				}
				// Upper South line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_UpperSW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_UpperSW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_UpperSW.pointXYZ.x	&& pointToCheck.x < in_blockBorderRef->corner_UpperSE.pointXYZ.x)			// is x between the UpperSW and UpperSE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_upperSouth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 3) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_upperSouth);
				}
				// Upper North line check-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				else if ((pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y)
					&&
					(pointToCheck.z == in_blockBorderRef->corner_UpperNW.pointXYZ.z)
					&&
					(pointToCheck.x > in_blockBorderRef->corner_UpperNW.pointXYZ.x	&& pointToCheck.x < in_blockBorderRef->corner_UpperNE.pointXYZ.x)			// is x between the UpperNW and UpperNE corners, but not equal to either of them?
					)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->Xaxis_upperNorth, in_originPoint, pointToCheck);	// get the shifting key
					//std::cout << " >>>> Y equals Z (branch 1, 4) " << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->Xaxis_upperNorth);
				}
			}

			// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			// otherwise, the non-twin of X is the closest intercept. So just use it's coords, and don't round any points to line or corner.
			else if ((dist_to_X < dist_to_Y) && (in_slopeDirection.x != 0.0f))
			{
				//std::cout << " >>>> Y equals Z (branch 2) " << std::endl;
				pointToCheck = x_intercept_coords;
				//std::cout << " |||| (branch 2, 2) Point to check values: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
				if (in_slopeDirection.x == 1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->EastFace, in_originPoint, pointToCheck);
					//std::cout << "(branch 2), East face" << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
				}
				else if (in_slopeDirection.x == -1.0f)
				{
					//newKey = OrganicUtils::getBorderShiftResult(in_blockBorderRef->WestFace, in_originPoint, pointToCheck);
					//std::cout << "(branch 2), West face" << std::endl;
					calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
				}
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for X, but Y and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_X != 0.0f) && (dist_to_Y == dist_to_Z) && (dist_to_Y == 0.0f))
		{
			//std::cout << "CONDITION 4 MET" << std::endl;
			pointToCheck = x_intercept_coords;
			if ((pointToCheck.x == in_blockBorderRef->corner_LowerNW.pointXYZ.x))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
			}
			else if ((pointToCheck.x == in_blockBorderRef->corner_LowerNE.pointXYZ.x))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
			}


		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 5: checking for Y, but X and Z are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Y != 0.0f) && (dist_to_X == dist_to_Z) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 5 MET " << std::endl;
			pointToCheck = y_intercept_coords;
			if ((pointToCheck.y == in_blockBorderRef->corner_LowerNW.pointXYZ.y))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
			}
			else if ((pointToCheck.y == in_blockBorderRef->corner_UpperNW.pointXYZ.y))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
			}
		}

		// ||||||||||||||||||||||||||||||||||||||||| condition 6: checking for Z, but X and Y are both 0.0f ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
		else if ((dist_to_Z != 0.0f) && (dist_to_X == dist_to_Y) && (dist_to_X == 0.0f))
		{
			//std::cout << "CONDITION 6 MET" << std::endl;
			pointToCheck = z_intercept_coords;
			if ((pointToCheck.z == in_blockBorderRef->corner_LowerNW.pointXYZ.z))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
			}
			else if ((pointToCheck.z == in_blockBorderRef->corner_LowerSW.pointXYZ.z))
			{
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
			}

		}





		else
		{
			//std::cout << "NO MATCH TO ANYTHING " << std::endl;
		}
	}

	// Step 3: else, it's just a normal intercept
	else {
		//std::cout << "::::NORMAL INTERCEPT DETECTED::::" << std::endl;
		isPointOnALine = 0;		// reset this (needs a fix, 1/21/2019)
		//std::cout << "Value of is point on a line: " << isPointOnALine << std::endl;
		float currentShortestTime = 0.0f;
		int currentShortestIndex = -1;		// -1 is default (no valid index selected)
		float distanceValues[3];			// set up a temp array
		distanceValues[0] = in_distanceValues.x;	// store x distance value
		distanceValues[1] = in_distanceValues.y;	// "" y
		distanceValues[2] = in_distanceValues.z;	// "" z

		for (int x = 0; x < 3; x++)
		{
			if (distanceValues[x] != 0.0f)	// is there an actual valid time for this?
			{
				if (currentShortestTime == 0.0f)	// for when the current length is 0 (this condition should always be met)
				{
					currentShortestTime = distanceValues[x];	// set the initial length
					currentShortestIndex = x;							// set the index; 0 = x, 1 = y, 2 = z;
				}
				else if (currentShortestTime != 0.0f)
				{
					if (distanceValues[x] < currentShortestTime)	// is the length being checked even smaller than the currentShortestLength?
					{
						currentShortestTime = distanceValues[x];	// reset the length
						currentShortestIndex = x;							// reset the index
					}
				}
			}
		}
		if (currentShortestIndex == 0)	// x was found
		{
			pointToCheck = x_intercept_coords;
			if (in_slopeDirection.x == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.EastFace, in_originPoint, pointToCheck);
				//::cout << "Type 3: line intersects at +X" << std::endl;
				//calculatedEndpointData.moveDirectionValues = in_blockBorderRef->EastFace.
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->EastFace);
			}
			else if (in_slopeDirection.x == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.WestFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -X" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->WestFace);
			}
		}
		else if (currentShortestIndex == 1)		// y was found
		{
			pointToCheck = y_intercept_coords;
			if (in_slopeDirection.y == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.TopFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at +Y" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->TopFace);
			}
			else if (in_slopeDirection.y == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.BottomFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -Y" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->BottomFace);
			}
		}
		else if (currentShortestIndex == 2)		// z was found
		{
			pointToCheck = z_intercept_coords;
			if (in_slopeDirection.z == 1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.SouthFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at +Z" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->SouthFace);
			}
			else if (in_slopeDirection.z == -1)
			{
				//newKey = OrganicUtils::getBorderShiftResult(pointABorderLineList.NorthFace, in_originPoint, pointToCheck);
				//std::cout << "Type 3: line intersects at -Z" << std::endl;
				calculatedEndpointData.applyBlockBorder(in_blockBorderRef->NorthFace);
			}
		}

	}
	//std::cout << "whoa" << std::endl;

	if (calculatedEndpointData.moveDirectionValues.isAllZero() == true)
	{
		endpointTracer.printTrace();
	}

	calculatedEndpointData.pointLocation = pointToCheck;
	calculatedEndpointData.matchSlopesToDirections(in_slopeDirection);

	return calculatedEndpointData;
}

ECBPolyPoint RasterCubeTracerBase::roundXYZInterceptDistancesToAppropriatePrecision(float in_distX, float in_distY, float in_distZ)
{
	ECBPolyPoint returnPoint;
	struct DistTracker
	{
		float distance;	// stores the passed in distance to x/y/z
		int dimension;	// represents dimension type, 0 = x, 1 = y, 2 = z
	};
	DistTracker tracker[3];			// create an array
	float edgeThreshold = 0.05f;	// set the threshold value; used to prevent F-007

	// initialize array
	tracker[0].distance = in_distX;
	tracker[1].distance = in_distY;
	tracker[2].distance = in_distZ;

	// cycle through and determine the candidate count, while also setting dimension values
	int candidateCount = 3;
	for (int x = 0; x < 3; x++)
	{
		tracker[x].dimension = x;
		if (tracker[x].distance == 0.0f)
		{
			candidateCount--;	// subtract as a candidate if it is 0 
		}
	}

	// do stuff based on number of candidates
	if (candidateCount == 3)		// it will be 3, rarely
	{
		//std::cout << ">>>>> Candidate count  is 3" << std::endl;
		DistTracker tripleTracker[3];
		float leastValue = 0.0f;
		for (int x = 0; x < 3; x++)			// initialize all rows
		{
			tripleTracker[x] = tracker[x];
		}

		// order from least to greatest
		//leastValue = tripleTracker[0].distance;	// initiate the first least value
		//int leastIndex = 0;
		for (int a = 0; a < 2; a++)
		{
			for (int b = a; b < 2; b++)
			{
				if (tripleTracker[a].distance >= tripleTracker[b + 1].distance)		// is the current "least" greater than the selected value? if so, then we must swap
				{
					DistTracker tempTracker = tripleTracker[a];
					tripleTracker[a] = tripleTracker[b + 1];
					tripleTracker[b + 1] = tempTracker;
				}
			}
		}

		// check relationship between 0 and 1
		for (int j = 0; j < 2; j++)
		{
			for (int z = 0; z < 2; z++)
			{
				//std::cout << "!! Comparing value at index " << z << " to value at index " << z + 1 << std::endl;

				float numeratorFloat = tripleTracker[z].distance;
				float denominatorFloat = tripleTracker[z + 1].distance;

				//std::cout << "3 candidates, :: numerator float   -> " << numeratorFloat << std::endl;
				//std::cout << "3 candidates, :: denominator float -> " << denominatorFloat << std::endl;



				float ratioResult = numeratorFloat / denominatorFloat;
				if (ratioResult > 1.0f)
				{
					//std::cout << "ratioResult > 1.0f hit. " << std::endl;

					float resultantThreshold = ratioResult - 1.0f;
					if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
					{
						//std::cout << ": > 1.0f, passed threshold check. " << std::endl;
						tripleTracker[z].distance = tripleTracker[z + 1].distance;	// set the distances the same
						//std::cout << "3 candidates: ratio > 1.0f" << std::endl;
					}

				}
				else if (ratioResult < 1.0f)
				{
					//std::cout << "ratioResult < 1.0f hit. " << std::endl;

					float resultantThreshold = 1.0f - ratioResult;
					//std::cout << "ratioResult is: " << ratioResult << std::endl;
					//std::cout << "resultantThreshold is: " << resultantThreshold << std::endl;

					//std::cout << "branch 2, resultantThreshold: " << resultantThreshold << std::endl;
					if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
					{
						tripleTracker[z].distance = tripleTracker[z + 1].distance;	// set the distances the same
						//std::cout << ": < 1.0f, passed threshold check. " << std::endl;
						//std::cout << "3 candidates: ratio < 1.0f" << std::endl;
					}
				}

			}
		}

		// populate values from tripleTracker into tracker
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (tripleTracker[x].dimension == tracker[y].dimension)
				{
					tracker[y].distance = tripleTracker[x].distance;
					//std::cout << "ratio < 1.0f" << std::endl;
				}
			}
		}



	}

	else if (candidateCount == 2)	// it will be 2, most of the time
	{
		//std::cout << ">>>>> Candidate count  is 2" << std::endl;
		DistTracker duoTracker[2];	// will store the two candidates
		int trackerIndex = 0;		// index, incremented by 1 after first is found
		for (int x = 0; x < 3; x++)
		{
			if (tracker[x].distance != 0.0f)
			{
				duoTracker[trackerIndex] = tracker[x];
				trackerIndex++;
			}
		}

		// now, check the relationship between both
		float numeratorFloat = duoTracker[0].distance;
		float denominatorFloat = duoTracker[1].distance;

		//std::cout << ":: numerator float   -> " << numeratorFloat << std::endl;
		//std::cout << ":: denominator float -> " << denominatorFloat << std::endl;

		// calculate the ratio
		float ratioResult = numeratorFloat / denominatorFloat;
		//std::cout << ":: Ratio result is: ---> " << ratioResult;
		if (ratioResult > 1.0f)
		{
			//std::cout << "Ratio result entered > 1 branch... " << std::endl;
			float resultantThreshold = ratioResult - 1.0f;
			//std::cout << "Resultant threshold is: " << resultantThreshold << std::endl;

			if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
			{
				duoTracker[0].distance = duoTracker[1].distance;	// set the distances the same
				//std::cout << "ratio > 1.0f" << std::endl;
			}

		}
		else if (ratioResult < 1.0f)
		{
			float resultantThreshold = 1.0f - ratioResult;
			if (resultantThreshold < edgeThreshold)		// if it's within the threshold, go ahead and make the distances the same (order of this doesn't matter with 2 distances)
			{
				duoTracker[0].distance = duoTracker[1].distance;	// set the distances the same
				//std::cout << "ratio < 1.0f" << std::endl;
			}
		}


		// populate values from duoTracker into tracker
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (duoTracker[x].dimension == tracker[y].dimension)	// do this dimensions match?
				{
					tracker[y].distance = duoTracker[x].distance;		// ok, so replace them
				}
			}
		}



	}

	returnPoint.x = tracker[0].distance;
	returnPoint.y = tracker[1].distance;
	returnPoint.z = tracker[2].distance;



	//std::cout << "Return distance values: " << std::endl;
	//std::cout << returnPoint.x << std::endl;
	//std::cout << returnPoint.y << std::endl;
	//std::cout << returnPoint.z << std::endl;

	//std::cout << "Distance values, post change: " << returnPoint.x << ", " << returnPoint.y << ",  " << returnPoint.z << std::endl;


	// DEBUG ONLY, remove when done
	/*
	if
		(
		(in_distX >= .00308642f)
			&&
			(in_distX <= .00308646f)
			)
	{
		std::cout << "Halting via in_distX; " << std::endl;
		int someVal = 3;
		std::cin >> someVal;
	}
	*/

	//std::cout << "Returning precisions..." << std::endl;
	return returnPoint;
}

ECBPolyPoint RasterCubeTracerBase::bindToNearestCorner(int in_xoryorz, ECBPolyPoint in_polyPoint)
{
	ECBPolyPoint calibratedPoint = getTrimmedPoint(in_polyPoint);

	// Round other points to X
	if (in_xoryorz == 0)
	{
		//if (in_polyPoint.x == rasterCubeDimLength)		// is x == 1.0?
		//{
			// check for y 
		if (in_polyPoint.y > (rasterCubeDimLength / 2))	// is y greater than (rasterCubeDimLength / 2)? if yes, then round to rasterCubeDimLength
		{
			calibratedPoint.y = rasterCubeDimLength;
		}
		else if (in_polyPoint.y < (rasterCubeDimLength / 2))
		{
			calibratedPoint.y = 0.0f;
		}

		// check for z
		if (in_polyPoint.z > (rasterCubeDimLength / 2)) // is z greater than (rasterCubeDimLength / 2)? if yes, then round to rasterCubeDimLength
		{
			calibratedPoint.z = rasterCubeDimLength;
		}
		else if (in_polyPoint.z < (rasterCubeDimLength / 2))
		{
			calibratedPoint.z = 0.0f;
		}
		//}
	}

	// Round other points to Y
	else if (in_xoryorz == 1)	// otherwise, point is on a line
	{
		// check for x
		if (in_polyPoint.x > (rasterCubeDimLength / 2))
		{
			calibratedPoint.x = rasterCubeDimLength;
		}
		else if (in_polyPoint.x < (rasterCubeDimLength / 2))
		{
			calibratedPoint.x = 0.0f;
		}

		// check for z
		if (in_polyPoint.z > (rasterCubeDimLength / 2))
		{
			calibratedPoint.z = rasterCubeDimLength;
		}
		else if (in_polyPoint.z < (rasterCubeDimLength / 2))
		{
			calibratedPoint.z = 0.0f;
		}
	}

	// Round other points to Z
	else if (in_xoryorz == 2)
	{
		// check for x
		if (in_polyPoint.x > (rasterCubeDimLength / 2))
		{
			calibratedPoint.x = rasterCubeDimLength;
		}
		else if (in_polyPoint.x < (rasterCubeDimLength / 2))
		{
			calibratedPoint.x = 0.0f;
		}

		// check for y
		if (in_polyPoint.y > (rasterCubeDimLength / 2))	// is y greater than (rasterCubeDimLength / 2)? if yes, then round to rasterCubeDimLength
		{
			calibratedPoint.y = rasterCubeDimLength;
		}
		else if (in_polyPoint.y < (rasterCubeDimLength / 2))
		{
			calibratedPoint.y = 0.0f;
		}
	}

	return calibratedPoint;
}

ECBPolyPoint RasterCubeTracerBase::bindToNearestLine(int in_xoryorz, 
	ECBPolyPoint in_polyPoint,
	DynamicLinePointSyncExclusionFlags in_exclusionFlag)
{
	ECBPolyPoint calibratedPoint = getTrimmedPoint(in_polyPoint);

	int numberOfCalibrations = 0;	// any valid calibration will give this value something greater than 0
	if (in_xoryorz == 0)
	{
		DynamicLinePointSynchronizer pointSync(calibratedPoint, 0, rasterCubeDimLength, in_exclusionFlag);
		//DynamicLineSyncResult xSyncResult = pointSync.sync();
		//calibratedPoint = xSyncResult.syncPoint;

		calibratedPoint = pointSync.sync();
	}

	// Solve for X and Z
	else if (in_xoryorz == 1)
	{
		DynamicLinePointSynchronizer pointSync(calibratedPoint, 1, rasterCubeDimLength, in_exclusionFlag);
		//DynamicLineSyncResult ySyncResult = pointSync.sync();
		//calibratedPoint = ySyncResult.syncPoint;
		calibratedPoint = pointSync.sync();

	}

	// check for x
	else if (in_xoryorz == 2)
	{
		DynamicLinePointSynchronizer pointSync(calibratedPoint, 2, rasterCubeDimLength, in_exclusionFlag);
		//DynamicLineSyncResult zSyncResult = pointSync.sync();
		//calibratedPoint = zSyncResult.syncPoint;
		calibratedPoint = pointSync.sync();
	}

	if (numberOfCalibrations == 0)
	{
		//std::cout << "!!!! WARNING: improper calibration detected. " << std::endl;
		//std::cout << "!!!! point is: " << in_polyPoint.x << ", " << in_polyPoint.y << ", " << in_polyPoint.z << ", " << std::endl;
		//std::cout << "!!!! calibrated point is: " << calibratedPoint.x << ", " << calibratedPoint.y << ", " << calibratedPoint.z << std::endl;
		//std::cout << "!!!! in_xoryorz: " << in_xoryorz << std::endl;
		//int stopVal;
		//std::cin >> stopVal;
	}

	return calibratedPoint;
}

ECBPolyPoint RasterCubeTracerBase::getTrimmedPoint(ECBPolyPoint in_polyPoint)
{
	ECBPolyPoint calibratedPoint = in_polyPoint;

	if (calibratedPoint.x < 0.0f)
	{
		calibratedPoint.x = 0.0f;
	}
	else if (calibratedPoint.x > rasterCubeDimLength)
	{
		calibratedPoint.x = rasterCubeDimLength;
	}

	// corrections for Y
	if (calibratedPoint.y < 0.0f)
	{
		calibratedPoint.y = 0.0f;
	}
	else if (calibratedPoint.y > rasterCubeDimLength)
	{
		calibratedPoint.y = rasterCubeDimLength;
	}

	// corrections for Z
	if (calibratedPoint.z < 0.0f)
	{
		calibratedPoint.z = 0.0f;
	}
	else if (calibratedPoint.z > rasterCubeDimLength)
	{
		calibratedPoint.z = rasterCubeDimLength;
	}
	return calibratedPoint;
}