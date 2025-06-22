#include "stdafx.h"
#include "STriangle.h"

STriangle::STriangle(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_pointC)
{
	//triangleLines[0].pointA = in_pointA;
	//triangleLines[0].pointB = in_pointB;
	//triangleLines[1].pointA = in_pointB;
	//triangleLines[1].pointB = in_pointC;
	//triangleLines[2].pointA = in_pointC;
	//triangleLines[2].pointB = in_pointA;

	triangleLines[0].setPointA(in_pointA.x, in_pointA.y, in_pointA.z);
	triangleLines[0].setPointB(in_pointB.x, in_pointB.y, in_pointB.z);
	
	triangleLines[1].setPointA(in_pointB.x, in_pointB.y, in_pointB.z);
	triangleLines[1].setPointB(in_pointC.x, in_pointC.y, in_pointC.z);
	
	triangleLines[2].setPointA(in_pointC.x, in_pointC.y, in_pointC.z);
	triangleLines[2].setPointB(in_pointA.x, in_pointA.y, in_pointA.z);

}

STriangle::STriangle()
{

}