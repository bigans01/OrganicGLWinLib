#include "stdafx.h"
#include "RenderablePrimitiveContainer.h"

void RenderablePrimitiveContainer::addPrimitive(std::shared_ptr<RenderablePrimitiveBase> primitiveToAdd)
{
	primitives.push_back(primitiveToAdd);
}

std::vector<GLfloat> RenderablePrimitiveContainer::buildContainerMesh()
{
	// iterate through each primitive, get it's total floats, add each float value to the vector.
	std::vector<GLfloat> floatVector;
	auto primitivesBegin = primitives.begin();
	auto primitivesEnd = primitives.end();
	for (; primitivesBegin != primitivesEnd; primitivesBegin++)
	{
		int currentTotalFloats = (*primitivesBegin)->getTotalFloats();
		auto currentDataPtr = (*primitivesBegin)->getArrayDataPtr();
		for (int x = 0; x < currentTotalFloats; x++)
		{
			floatVector.push_back(currentDataPtr[x]);
		}
	}

	//std::cout << "Size of vector is now: " << floatVector.size() << std::endl;
	return floatVector;
}