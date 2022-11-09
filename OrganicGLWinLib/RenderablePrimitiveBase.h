#pragma once

#ifndef RENDERABLEPRIMITIVEBASE_H
#define RENDERABLEPRIMITIVEBASE_H

// Description: a base class meant for basic 3d shapes (cube, sphere, cuboid, etc); 
// various attributes of the mesh can be constructed/accessed through base and virtual methods.
// It is primarily meant for use by OrganicCoreLib -- specifically the HighlighterContainer class in that library -- so that it may 
// manage these basic shapes.

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Triangle.h"

class RenderablePrimitiveBase
{
	public:
		RenderablePrimitiveBase& operator=(const RenderablePrimitiveBase& primitive_b)
		{
			color = primitive_b.color;
			totalTriangles = primitive_b.totalTriangles;
			totalFloats = primitive_b.totalFloats;
			if (primitive_b.totalFloats > 0)
			{
				pointArray.reset(new GLfloat[totalFloats]);
				for (int x = 0; x < totalFloats; x++)
				{
					pointArray[x] = primitive_b.pointArray[x];
				}
			}
			else if (primitive_b.totalFloats == 0)
			{
				pointArray.reset();
			}
			return *this;
		}

		// virtual methods that must be defined
		virtual void buildTriangles() = 0;								// Step 2: build the mesh, and the corresponding data

		// optional virtual methods (printing triangles, etc)

		// base methods
		void setColor(float in_red, float in_green, float in_blue);		// Step 1: (optional) set color; default is white
		void setColor(glm::vec3 in_color);
		void setCenter(float in_x, float in_y, float in_z);	// set the coordinate that represents the center of the primitive; other points 
															// will be built around this coordinate.
		void setDistanceFromCenter(float in_distance);		// set the initial distance from center, before the call to buildTriangles(); this
															// value will be multiplied the value of distScale.
		void setDistanceScaling(float in_scalingValue);		// set the distance multiplier, before the call to buildTriangles()
		void buildGLData();
		void printTriangles();
		int getTotalFloats();	// used by RenderablePrimitiveContainer class
		GLfloat* getArrayDataPtr();
	protected:
		glm::vec3 center = glm::vec3(0.5f, 0.5f, 0.5f);	// the center of the primitive, may be changed at will
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);								// the primitive's color used for OpenGL rendering
		float distFromCenter = 0.5f;	// how far each triangle should be from the "center"
		float distScale = 1.0f;			// can use this to shrink/expand points from the center of the cube; i.e., 0.5f makes for a smaller object, > 1.0f makes it larger
		int totalTriangles = 0;		// the total number of triangles in the primitive; i.e, 12 for a cuboid
		int totalFloats = 0;	// the total floats (for the unique_ptr)
		std::vector<Triangle> triangleVector;	// all the triangles that make up the mesh
		std::unique_ptr<GLfloat[]> pointArray;


		virtual void applyDistScaling() = 0;	// defined per derived-class; 

};

#endif
