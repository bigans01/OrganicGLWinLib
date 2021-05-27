#pragma once

#ifndef QMBOOLRESOLVECONTESTEDCATEGORIZEDLINE_H
#define QMBOOLRESOLVECONTESTEDCATEGORIZEDLINE_H

#include "QMBoolBase.h"

class QMBoolResolveContestedCategorizedLine : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void rotateContestedLineAroundXToPositiveY();
		void rotateContestedLinePointBAroundZToPositiveX();
		void rotateBoundaryCenterPointAlongXToPosOrNegY();
		glm::vec3* contestedLinePointBRef = nullptr;
		QuatRotationPoints* rotationPointsRefVector = nullptr;
};

#endif