#include "stdafx.h"
#include "RenderablePrimitiveBase.h"

void RenderablePrimitiveBase::setColor(float in_red, float in_green, float in_blue)
{
	color.x = in_red;
	color.y = in_green;
	color.z = in_blue;
}

void RenderablePrimitiveBase::setColor(glm::vec3 in_color)
{
	color = in_color;
}

void RenderablePrimitiveBase::buildGLData()
{

}

void RenderablePrimitiveBase::setPosition(float in_x, float in_y, float in_z)
{

}

void RenderablePrimitiveBase::printTriangles()
{
	auto trianglesBegin = triangleVector.begin();
	auto trianglesEnd = triangleVector.end();
	int currentTriangle = 0;
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		std::cout << "Triangle " << currentTriangle++ << ": ";
		trianglesBegin->printPoints();
		std::cout << std::endl;
	}
}