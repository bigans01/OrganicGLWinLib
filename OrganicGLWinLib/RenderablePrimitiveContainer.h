#pragma once

#ifndef RENDERABLEPRIMITIVECONTAINER_H
#define RENDERABLEPRIMITIVECONTAINER_H

#include <vector>
#include "RenderablePrimitiveBase.h"
#include <mutex>
#include <iostream>

class RenderablePrimitiveContainer
{
	public:
		void addPrimitive(std::shared_ptr<RenderablePrimitiveBase> primitiveToAdd);		// **** 
																						// NOTE: this function assumes the primitive to be added
																						// has been configured/setup/instantiated appropriately, before
																						// it is added. 
																						// ****
																						//
																						// will add a primitive and its associated data
																						// to the primitives vector; these primitives
																						// get processed during the call to the 
																						// buildContainerMesh() function below.
																						
		std::vector<GLfloat> buildContainerMesh();	// will produce the data to send to OpenGL, via vector.
	private:
		std::vector<std::shared_ptr<RenderablePrimitiveBase>> primitives;
};

#endif
