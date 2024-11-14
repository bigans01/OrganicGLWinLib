#pragma once

#ifndef HASHEDSTRIANGLELINE_H
#define HASHEDSTRINAGLELINE_H

#include "HashUtils.h"

/*

Description: Designed to be used in conjunction with HashedSTriangleLineCounter and SPolyShellValidityChecker, this class
takes in two glm::vec3 points to form a line that contains two hashes. The hashes are meant to uniquely represent the line.

*/

class HashedSTriangleLine
{
	public:
		HashedSTriangleLine() {};
		HashedSTriangleLine(glm::vec3 in_pointA, glm::vec3 in_pointB);

		bool doLinesMatch(HashedSTriangleLine* in_otherLine);	// returns true if hashAB matches either hashAB 
																// or hashBA in the referenced line.
		void printHashedLineStats();
	private:
		glm::vec3 pointA;
		glm::vec3 pointB;

		std::string hashAB;	// the hash of pointA, then pointB
		std::string hashBA; // the hash of pointB, then pointA

		void buildHashes();	// build hashAB and hashBA
};

#endif
