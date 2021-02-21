#pragma once

#ifndef QMBOOLISCOLINEARLINEACONTAINEDWITHINB_H
#define QMBOOLISCOLINEARLINEACONTAINEDWITHINB_H

#include "QMBoolBase.h"

class QMBoolIsColinearLineAContainedWithinB : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void rotateLineToYZeroPositiveX(
											glm::vec3* in_pointToRotateFor, 
											std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, 
											QuatRotationPoints* in_quatRotationPointsRef, 
											std::vector<QuatRotationType>* in_rotationOrderVectorRef
										);
		void roundVec3XByTenThousandths(glm::vec3* in_vec3Ref);
		bool checkIfLineAPointIsContainedWithinLineB(glm::vec3 in_pointToCheck, QuatRotationPoints* in_quatRotationPointsRef);
};

#endif