#include "stdafx.h"
#include "STriangle.h"

STriangle::STriangle(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_pointC)
{
	triangleLines[0].pointA = in_pointA;
	triangleLines[0].pointB = in_pointB;
	triangleLines[1].pointA = in_pointB;
	triangleLines[1].pointB = in_pointC;
	triangleLines[2].pointA = in_pointC;
	triangleLines[2].pointB = in_pointA;
}

STriangle::STriangle()
{

}