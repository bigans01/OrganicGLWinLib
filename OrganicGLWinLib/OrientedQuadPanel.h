#pragma once

#ifndef ORIENTEDQUADPANEL_H
#define ORIENTEDQUADPANEL_H

#include "OrientedQuad.h"
#include "OrientedQuadPlane.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "OrganicGLWinUtils.h"
#include "Triangle.h"

class OrientedQuadPanel
{
	public:
		void setInitialPanelData(OrientedQuadPlane in_panelPlane, OrientedQuadQuadrant in_panelQuadrant);
		OrientedQuadPlane panelPlane = OrientedQuadPlane::NONE;				// the axis plane that the panel is on 
		OrientedQuadQuadrant panelQuadrant = OrientedQuadQuadrant::NONE;	// the panel default is none
		glm::vec3 panelOriginPoint;
		glm::vec3 direction1;	// the normalized direction of the first quadrant,	used to determine the panel quadrant's orientation
		glm::vec3 direction2;	// the normalized direction of the second quadrant,	used to determine the panel quadrant's orientation
		float panelLength = 0.0f;	// it's not really a "length", but meh

		std::vector<OrientedQuad> coreQuad;		// the quad at the center of the panel. this is the only quad that has a point which is shared among the other two OrientedQuadPanels, the "origin" in a HighlighterCorner (see OrganicCoreLib)
		std::vector<OrientedQuad> dir1Quads;	
		std::vector<OrientedQuad> dir2Quads;
		std::vector<glm::vec3*> pointList;		// a list of all points in this panel.

		void printDirections();
		void loadPointList();
		int loadTrianglesIntoVector(std::vector<Triangle>* in_triangleVectorRef);
		void printPoints();
		void clearQuadVectors();
		void createInitialQuads(float in_axisWidth, float in_axisLength);
		void applyQuaternionToPointList(glm::quat in_quaternion);
		void applyTranslationToPointList(glm::vec3 in_translationVector);

	private:
		void determineInitialDirections();
		void recalculatePlaneAndQuadrant();
		void fetchQuadPointRefs(std::vector<OrientedQuad>* in_quadVectorRef);
};

#endif
