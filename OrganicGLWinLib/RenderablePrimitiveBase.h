#pragma once

#ifndef RENDERABLEPRIMITIVEBASE_H
#define RENDERABLEPRIMITIVEBASE_H

// Description: a base class meant for basic 3d shapes (cube, sphere, cuboid, etc); 
// various attributes of the mesh can be constructed/accessed through base and virtual methods.
// It is primarily meant for use by OrganicCoreLib -- specifically the HighlighterContainer class in that library -- so that it may 
// manage these basic shapes.

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class RenderablePrimitiveBase
{
	public:
		// virtual methods
		virtual void buildTriangles() = 0;

		// base methods
		void setColor(float in_red, float in_green, float in_blue);
		void buildGLData();
		void setPosition(float in_x, float in_y, float in_z);

};

#endif
