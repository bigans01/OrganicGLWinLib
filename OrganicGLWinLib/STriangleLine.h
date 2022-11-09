#pragma once

#ifndef STRIANGLELINE_H
#define STRIANGLELINE_H


class STriangleLine
{
public:
	glm::vec3 pointA;	// the beginning of the line
	glm::vec3 pointB;	// the end of the line
	unsigned char isBorderLine = 0;		// indicates whether or not it is a border line of the STriangleSet this triangle belongs in
	unsigned char borderLineID = 0;		// indicates the borderLineID, if it is indeed a border line
	unsigned char pointAOnBorderLine = 0;	// is point A on border line? (checked when borderLineID is 1)
	unsigned char pointBOnBorderLine = 0;	// is point B on border line? " " " 
};

#endif
