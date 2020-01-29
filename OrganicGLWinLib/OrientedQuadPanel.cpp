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
	// load the direction vectors
	pointList.push_back(&direction1);
	pointList.push_back(&direction2);
}

void OrientedQuadPanel::applyQuaternionToPointList(glm::quat in_quaternion)
{
	auto vectorBegin = pointList.begin();
	auto vectorEnd = pointList.end();
	for (vectorBegin; vectorBegin != vectorEnd; vectorBegin++)
	{
		**vectorBegin = in_quaternion * **vectorBegin;
		std::cout << "!!!! Quaternion applied!" << std::endl;
	}
}