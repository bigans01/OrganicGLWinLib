#include "stdafx.h"
#include "CoplanarMassManipulator.h"

//void CoplanarMassManipulator::initialize(SPoly* in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap, QuatRotationPoints* in_coplanarPointsRef)
void CoplanarMassManipulator::initialize(SPoly in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap, QuatRotationPoints* in_coplanarPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	trackedSPolyRef = in_trackedSPolyRef;
	sPolyRefMap = in_sPolyRefMap;
	coplanarPointsRef = in_coplanarPointsRef;
	massManipulatorLogLevel = in_polyDebugLevel;
	massManipulatorLogger.setDebugLevel(in_polyDebugLevel);
}

float CoplanarMassManipulator::calculateSPolyArea(SPoly* in_sPolyRef)
{
	// area of an SPoly is equivalent to the combined area of all its STriangles.
	float totalArea = 0.0f;
	auto sTrianglesBegin = in_sPolyRef->triangles.begin();
	auto sTrianglesEnd = in_sPolyRef->triangles.end();
	for (; sTrianglesBegin != sTrianglesEnd; sTrianglesBegin++)
	{
		totalArea += calculateTriangleArea(sTrianglesBegin->second.triangleLines[0].pointA, sTrianglesBegin->second.triangleLines[1].pointA, sTrianglesBegin->second.triangleLines[2].pointA);
	}
	return totalArea;
}

float CoplanarMassManipulator::calculateTriangleArea(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	// find distance between 
	float line1 = glm::distance(in_point0, in_point1);
	float line2 = glm::distance(in_point1, in_point2);
	float line3 = glm::distance(in_point2, in_point0);

	// Heron's formula
	float s = .5 * (line1 + line2 + line3);
	float A = sqrt(s * (s - line1) * (s - line2) * (s - line3));
	
	

	std::cout << "Area is: " << A << std::endl;

	return A;
}

double CoplanarMassManipulator::calculateTriangleAreaDouble(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	// find distance between 
	float line1 = glm::distance(in_point0, in_point1);
	float line2 = glm::distance(in_point1, in_point2);
	float line3 = glm::distance(in_point2, in_point0);

	// Heron's formula (doubles)
	double s = .5 * (line1 + line2 + line3);
	double A = sqrtl(s * (s - line1) * (s - line2) * (s - line3));

	return A;
}