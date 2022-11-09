#pragma once

#ifndef SPOLYBORDERLINES_H
#define SPOLYBORDERLINES_H

#include "BorderLineIntersectRecorder.h"

class SPolyBorderLines
{
public:
	glm::vec3 pointA;
	glm::vec3 pointB;
	glm::vec3 planarVector;
	BorderLineIntersectRecorder intersectRecorder;
};

#endif