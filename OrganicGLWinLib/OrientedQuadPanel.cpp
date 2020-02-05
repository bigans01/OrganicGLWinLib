#include "stdafx.h"
#include "OrientedQuadPanel.h"

void OrientedQuadPanel::setInitialPanelData(OrientedQuadPlane in_panelPlane, OrientedQuadQuadrant in_panelQuadrant)
{
	panelPlane = in_panelPlane;
	panelQuadrant = in_panelQuadrant;
	//direction1 = in_direction1;
	//direction2 = in_direction2;
	determineInitialDirections();

}

void OrientedQuadPanel::createInitialQuads(float in_coreDim, float in_axisLength, float in_axisWidth)
{
	glm::vec3 someVec;
	someVec.x = 0;
	someVec.y = 0;
	someVec.z = 0;

	// set the lengths
	panelLength = in_axisLength;
	float nonCoreLength = panelLength - in_coreDim;	// nonCoreLength is going to be the length of the non core quads. If the core quad has a .1 dim value, but the length of the total panel is 4.0f, the non core quad would be 3.99f.

	// construct the core quad
	OrientedQuad newCoreQuad(someVec, in_coreDim, direction1, direction2);	// construct the square
	coreQuad.push_back(newCoreQuad);

	// construct the dir1Quads
	OrientedQuad dir1Quad(coreQuad.begin()->getPoint(1), in_coreDim, nonCoreLength, direction1, direction2);	// point 1 = on same trajectory as direction 1
	coreQuad.push_back(dir1Quad);

	OrientedQuad dir2Quad(coreQuad.begin()->getPoint(3), in_coreDim, nonCoreLength, direction2, direction1);	// point 3 = on same trajectory as direction 2
	coreQuad.push_back(dir2Quad);

	std::cout << "---------------------------------------" << std::endl;
}

void OrientedQuadPanel::determineInitialDirections()
{
	if (panelPlane == OrientedQuadPlane::X)
	{
		if (panelQuadrant == OrientedQuadQuadrant::XPLANE_POSY_POSZ)
		{
			// reset first direction to +Y
			glm::vec3 dir1;
			dir1.y = 1.0f;
			direction1 = dir1;

			// reset first direction to +Z
			glm::vec3 dir2;
			dir2.z = 1.0f;
			direction2 = dir2;
		}
		else if (panelQuadrant == OrientedQuadQuadrant::XPLANE_POSY_NEGZ)
		{
			// reset first direction to +Y
			glm::vec3 dir1;
			dir1.y = 1.0f;
			direction1 = dir1;

			// reset first direction to -Z
			glm::vec3 dir2;
			dir2.z = -1.0f;
			direction2 = dir2;
		}
		else if (panelQuadrant == OrientedQuadQuadrant::XPLANE_NEGY_NEGZ)
		{
			// reset first direction to -Y
			glm::vec3 dir1;
			dir1.y = -1.0f;
			direction1 = dir1;

			// reset first direction to -Z
			glm::vec3 dir2;
			dir2.z = -1.0f;
			direction2 = dir2;
		}
		else if (panelQuadrant == OrientedQuadQuadrant::XPLANE_NEGY_POSZ)
		{
			// reset first direction to -Y
			glm::vec3 dir1;
			dir1.y = -1.0f;
			direction1 = dir1;

			// reset first direction to +Z
			glm::vec3 dir2;
			dir2.z = +1.0f;
			direction2 = dir2;
		}
	}
	else if (panelPlane == OrientedQuadPlane::Y)
	{
		if (panelQuadrant == OrientedQuadQuadrant::YPLANE_POSX_POSZ)
		{
			// reset first direction to +X
			glm::vec3 dir1;
			dir1.x = 1.0f;
			direction1 = dir1;

			// reset first direction to +Z
			glm::vec3 dir2;
			dir2.z = 1.0f;
			direction2 = dir2;
		}
		else if (panelQuadrant == OrientedQuadQuadrant::YPLANE_NEGX_POSZ)
		{
			// reset first direction to -X
			glm::vec3 dir1;
			dir1.x = -1.0f;
			direction1 = dir1;

			// reset first direction to +Z
			glm::vec3 dir2;
			dir2.z = 1.0f;
			direction2 = dir2;
		}
		else if (panelQuadrant == OrientedQuadQuadrant::YPLANE_NEGX_NEGZ)
		{
			// reset first direction to -X
			glm::vec3 dir1;
			dir1.x = -1.0f;
			direction1 = dir1;

			// reset first direction to -Z
			glm::vec3 dir2;
			dir2.z = -1.0f;
			direction2 = dir2;
		}
		else if (panelQuadrant == OrientedQuadQuadrant::YPLANE_NEGX_NEGZ)
		{
			// reset first direction to +X
			glm::vec3 dir1;
			dir1.x = 1.0f;
			direction1 = dir1;

			// reset first direction to -Z
			glm::vec3 dir2;
			dir2.z = -1.0f;
			direction2 = dir2;
		}
	}
	else if (panelPlane == OrientedQuadPlane::Z)
	{
		if (panelQuadrant == OrientedQuadQuadrant::ZPLANE_POSX_POSY)
		{
			// reset first direction to +X
			glm::vec3 dir1;
			dir1.x = 1.0f;
			direction1 = dir1;

			// reset second diection to +Y
			glm::vec3 dir2;
			dir2.y = 1.0f;
			direction2 = dir2;
		}

		else if (panelQuadrant == OrientedQuadQuadrant::ZPLANE_NEGX_POSY)
		{
			// reset first direction to -X
			glm::vec3 dir1;
			dir1.x = -1.0f;
			direction1 = dir1;

			// reset second diection to +Y
			glm::vec3 dir2;
			dir2.y = 1.0f;
			direction2 = dir2;
		}

		else if (panelQuadrant == OrientedQuadQuadrant::ZPLANE_NEGX_NEGY)
		{
			// reset first direction to -X
			glm::vec3 dir1;
			dir1.x = -1.0f;
			direction1 = dir1;

			// reset second diection to -Y
			glm::vec3 dir2;
			dir2.y = -1.0f;
			direction2 = dir2;
		}
		else if (panelQuadrant == OrientedQuadQuadrant::ZPLANE_POSX_NEGY)
		{
			// reset first direction to +X
			glm::vec3 dir1;
			dir1.x = 1.0f;
			direction1 = dir1;

			// reset second diection to -Y
			glm::vec3 dir2;
			dir2.y = -1.0f;
			direction2 = dir2;
		}
	}
}

void OrientedQuadPanel::printDirections()
{
	std::cout << "!!! Printing directions: " << std::endl;
	std::cout << "Dir 1: " << direction1.x << ", " << direction1.y << ", " << direction1.z << std::endl;
	std::cout << "Dir 2: " << direction2.x << ", " << direction2.y << ", " << direction2.z << std::endl;
}

void OrientedQuadPanel::loadPointList()
{
	pointList.clear();

	// load the direction vectors
	pointList.push_back(&direction1);
	pointList.push_back(&direction2);

	// load the point refs from the quads in each directional vector
	fetchQuadPointRefs(&dir1Quads);	
	fetchQuadPointRefs(&dir2Quads);	
}

void OrientedQuadPanel::fetchQuadPointRefs(std::vector<OrientedQuad>* in_quadVectorRef)
{
	auto vectorBegin = in_quadVectorRef->begin();
	auto vectorEnd = in_quadVectorRef->end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		// there are 4 points per quad, get their references
		for (int x = 0; x < 4; x++)
		{
			pointList.push_back(vectorBegin->getPointRef(x));
		}
	}
}

void OrientedQuadPanel::applyQuaternionToPointList(glm::quat in_quaternion)
{
	// recalculate-the orientations
	auto vectorBegin = pointList.begin();
	auto vectorEnd = pointList.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		**vectorBegin = in_quaternion * **vectorBegin;
		std::cout << "!!!! --> Quaternion applied!" << std::endl;

		// normalize the vector here
		glm::vec3 roundedVector = OrganicGLWinUtils::roundDirectionalVector(**vectorBegin);
		**vectorBegin = roundedVector;
	}

	recalculatePlaneAndQuadrant();
	std::cout << "Post recalculate..." << std::endl;

}

void OrientedQuadPanel::recalculatePlaneAndQuadrant()
{
	std::cout << "!!!!!! Recalculating quadrant..." << std::endl;

	std::cout << ":: Direction 1: " << direction1.x << ", " << direction1.y << ", " << direction1.z << std::endl;
	std::cout << ":: Direction 2: " << direction2.x << ", " << direction2.y << ", " << direction2.z << std::endl;

	// ---------------------------- X plane calcs

	// XPLANE_POSY_POSZ
	if
	(
		((direction1.y == 1.0f) && (direction2.z == 1.0f))
		||
		((direction2.y == 1.0f) && (direction1.z == 1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a XPLANE_POSY_POSZ! " << std::endl;
		panelPlane = OrientedQuadPlane::X;
		panelQuadrant = OrientedQuadQuadrant::XPLANE_POSY_POSZ;
	}

	// XPLANE_NEGY_POSZ
	else if
	(
		((direction1.y == -1.0f) && (direction2.z == 1.0f))
		||
		((direction2.y == -1.0f) && (direction1.z == 1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a XPLANE_NEGY_POSZ! " << std::endl;
		panelPlane = OrientedQuadPlane::X;
		panelQuadrant = OrientedQuadQuadrant::XPLANE_NEGY_POSZ;
	}

	// XPLANE_NEGY_NEGZ
	else if
	(
		((direction1.y == -1.0f) && (direction2.z == -1.0f))
		||
		((direction2.y == -1.0f) && (direction1.z == -1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a XPLANE_NEGY_NEGZ! " << std::endl;
		panelPlane = OrientedQuadPlane::X;
		panelQuadrant = OrientedQuadQuadrant::XPLANE_NEGY_NEGZ;
	}

	// XPLANE_POSY_NEGZ
	else if
	(
		((direction1.y == 1.0f) && (direction2.z == -1.0f))
		||
		((direction2.y == 1.0f) && (direction1.z == -1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a XPLANE_POSY_NEGZ! " << std::endl;
		panelPlane = OrientedQuadPlane::X;
		panelQuadrant = OrientedQuadQuadrant::XPLANE_POSY_NEGZ;
	}

	// ---------------------------- Y plane calcs

	// YPLANE_POSX_POSZ
	else if
	(
		((direction1.x == 1.0f) && (direction2.z == 1.0f))
		||
		((direction2.x == 1.0f) && (direction1.z == 1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a YPLANE_POSX_POSZ! " << std::endl;
		panelPlane = OrientedQuadPlane::Y;
		panelQuadrant = OrientedQuadQuadrant::YPLANE_POSX_POSZ;
	}

	// YPLANE_POSX_NEGZ
	else if
	(
		((direction1.x == 1.0f) && (direction2.z == -1.0f))
		||
		((direction2.x == 1.0f) && (direction1.z == -1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a YPLANE_POSX_NEGZ! " << std::endl;
		panelPlane = OrientedQuadPlane::Y;
		panelQuadrant = OrientedQuadQuadrant::YPLANE_POSX_NEGZ;
	}

	// YPLANE_NEGX_NEGZ
	else if
	(
		((direction1.x == -1.0f) && (direction2.z == -1.0f))
		||
		((direction2.x == -1.0f) && (direction1.z == -1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a YPLANE_NEGX_NEGZ! " << std::endl;
		panelPlane = OrientedQuadPlane::Y;
		panelQuadrant = OrientedQuadQuadrant::YPLANE_NEGX_NEGZ;
	}

	// YPLANE_NEGX_POSZ
	else if
	(
		((direction1.x == -1.0f) && (direction2.z == 1.0f))
		||
		((direction2.x == -1.0f) && (direction1.z == 1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a YPLANE_NEGX_POSZ! " << std::endl;
		panelPlane = OrientedQuadPlane::Y;
		panelQuadrant = OrientedQuadQuadrant::YPLANE_NEGX_POSZ;
	}


	// ---------------------------- Z plane calcs

	// ZPLANE_POSX_POSY
	else if 
	(
		((direction1.x == 1.0f) && (direction2.y == 1.0f))
		||
		((direction2.x == 1.0f) && (direction1.y == 1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a ZPLANE_POSX_POSY! " << std::endl;
		panelPlane = OrientedQuadPlane::Z;
		panelQuadrant = OrientedQuadQuadrant::ZPLANE_POSX_POSY;
	}

	// ZPLANE_POSX_NEGY
	else if
	(
		((direction1.x == 1.0f) && (direction2.y == -1.0f))
		||
		((direction2.x == 1.0f) && (direction1.y == -1.0f))
	)
	{
		std::cout << "!!! Quad panel is now a ZPLANE_POSX_NEGY! " << std::endl;
		panelPlane = OrientedQuadPlane::Z;
		panelQuadrant = OrientedQuadQuadrant::ZPLANE_POSX_NEGY;
	}

	// ZPLANE_NEGX_NEGY
	else if
	(
		((direction1.x == -1.0f) && (direction2.y == -1.0f))
		||
		((direction2.x == -1.0f) && (direction1.y == -1.0f))
		)
	{
		std::cout << "!!! Quad panel is now a ZPLANE_NEGX_NEGY! " << std::endl;
		panelPlane = OrientedQuadPlane::Z;
		panelQuadrant = OrientedQuadQuadrant::ZPLANE_NEGX_NEGY;
	}

	// ZPLANE_NEGX_POSY
	else if
		(
		((direction1.x == -1.0f) && (direction2.y == 1.0f))
			||
			((direction2.x == -1.0f) && (direction1.y == 1.0f))
			)
	{
		std::cout << "!!! Quad panel is now a ZPLANE_NEGX_POSY! " << std::endl;
		panelPlane = OrientedQuadPlane::Z;
		panelQuadrant = OrientedQuadQuadrant::ZPLANE_NEGX_POSY;
	}
}