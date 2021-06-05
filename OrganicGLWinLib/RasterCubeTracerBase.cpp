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

	glm::vec3 currentIterationBeginPoint = currentTracingPoint;

	// ------------------------------- get X values
	if (normalizedSlopeDirection.x > 0)
	{
		x_interceptCoord = rasterCubeDimLength;
		float origin_to_border_x_diff = (x_interceptCoord - currentIterationBeginPoint.x);
		time_to_complete_x_traversal = origin_to_border_x_diff / tracerDirectionVector.x;

		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (tracerDirectionVector.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_x_traversal);
	}
	else if (normalizedSlopeDirection.x < 0)
	{
		x_interceptCoord = 0.0f;
		float origin_to_border_x_diff = abs(x_interceptCoord - currentIterationBeginPoint.x);					// make sure to get absolute value for these two lines (WEST border)
		time_to_complete_x_traversal = abs(origin_to_border_x_diff / tracerDirectionVector.x);	// ""

		calculatedPoint_for_x.x = x_interceptCoord;
		calculatedPoint_for_x.y = currentIterationBeginPoint.y + (tracerDirectionVector.y * time_to_complete_x_traversal);
		calculatedPoint_for_x.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_x_traversal);
	}

	// """ y values
	if (normalizedSlopeDirection.y > 0)
	{
		y_interceptCoord = rasterCubeDimLength;
		float origin_to_border_y_diff = (y_interceptCoord - currentIterationBeginPoint.y);
		time_to_complete_x_traversal = origin_to_border_y_diff / tracerDirectionVector.y;

		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (tracerDirectionVector.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_y_traversal);
	}
	else if (normalizedSlopeDirection.y < 0)
	{
		y_interceptCoord = 0.0f;
		float origin_to_border_y_diff = abs(y_interceptCoord - currentIterationBeginPoint.y);
		time_to_complete_x_traversal = abs(origin_to_border_y_diff / tracerDirectionVector.y);

		calculatedPoint_for_y.x = currentIterationBeginPoint.x + (tracerDirectionVector.x * time_to_complete_y_traversal);
		calculatedPoint_for_y.y = y_interceptCoord;
		calculatedPoint_for_y.z = currentIterationBeginPoint.z + (tracerDirectionVector.z * time_to_complete_y_traversal);
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
	}
	else if (normalizedSlopeDirection.z < 0)
	{
		z_interceptCoord = 0.0f;
		float origin_to_border_z_diff = abs(z_interceptCoord - currentIterationBeginPoint.z);
		time_to_complete_z_traversal = abs(origin_to_border_z_diff / tracerDirectionVector.z);

		calculatedPoint_for_z.x = currentIterationBeginPoint.x + (tracerDirectionVector.x * time_to_complete_z_traversal);
		calculatedPoint_for_z.y = currentIterationBeginPoint.y + (tracerDirectionVector.y * time_to_complete_z_traversal);
		calculatedPoint_for_z.z = z_interceptCoord;
	}

	ECBPolyPointTri triPointParam;
	triPointParam.triPoints[0] = calculatedPoint_for_x;
	triPointParam.triPoints[1] = calculatedPoint_for_y;
	triPointParam.triPoints[2] = calculatedPoint_for_z;
	ECBPolyPoint distanceValues;
	distanceValues.x = time_to_complete_x_traversal;
	distanceValues.y = time_to_complete_y_traversal;
	distanceValues.z = time_to_complete_z_traversal;
}