#include "stdafx.h"
#include "GPUWorldCoordinate.h"

GPUWorldCoordinate::GPUWorldCoordinate()
{

}

void GPUWorldCoordinate::printCoordinate()
{
	std::cout << "GPU world coordinate, BP key: ";
	worldBPKey.printKey();
	std::cout << std::endl;

	std::cout << "GPU world coordinate, localized coordinate: ";
	std::cout << worldLocalizedPoint.x << ", " << worldLocalizedPoint.y << ", " << worldLocalizedPoint.z << std::endl;
}