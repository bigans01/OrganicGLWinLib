#pragma once

#ifndef RCOLLISIONPOINT_H
#define RCOLLISIONPOINT_H

#include "ECBPPOrientations.h"
#include "RCollisionPointState.h"

class RCollisionPoint
{
	public:
		RCollisionPoint() {};
		RCollisionPoint(ECBPPOrientations in_originalGridOrientation, glm::vec3 in_originalPoint) :
			originalGridOrientation(in_originalGridOrientation),
			originalValue(in_originalPoint),
			currentValue(in_originalPoint)
		{};

		void printPointAndOrientation()
		{
			std::cout << originalValue.x << ", " << originalValue.y << ", " << originalValue.z << " | Orientation: ";
			IndependentUtils::printOrientationEnum(originalGridOrientation);
		}

		void incrementUsageCount();

		bool isPointLandlocked()
		{
			bool isLandlocked = false;
			if (pointState == RCollisionPointState::IMMUTABLE)
			{
				isLandlocked = true;
			}
			return isLandlocked;
		}

		void setPointAsBeingWithinMass()
		{
			if (pointState != RCollisionPointState::IMMUTABLE)
			{
				pointState = RCollisionPointState::WITHIN_MASS;
			}
		}

		void setCollisionPointMaterial(short in_sampledMaterialID)
		{
			sampledMaterialID = in_sampledMaterialID;
		}

		RCollisionPointState getCollisionPointState()
		{
			return pointState;
		}

		void setCollisionPointState(RCollisionPointState in_pointState)
		{
			pointState = in_pointState;
		}

		glm::vec3 originalValue;	// will always stay the same after it has been set
		glm::vec3 currentValue;		// can be manipulated by any MorphableMesh that has this as a point
		short sampledMaterialID = 0;	// the materialID that the point acquired, from the MassGridArrayCell that it ended up colliding (coming into contact) with
	private:
		int usageCount = 0;			// the number of times this point is used by instance(s) of RMorphableMesh; a value of 8 means the point should go unused (the point would be completely surrounded and not visible at all)
		RCollisionPointState pointState = RCollisionPointState::FREE;	// the point can be freely moved; all points start out as this
		bool massHit = false;		// would determine if the point collided with some form of matter during a trace by a MorphableMesh
		bool xMovementPermitted = true;
		bool yMovementPermitted = true;
		bool zMovementPermitted = true;
		ECBPPOrientations originalGridOrientation; // the point's original orientation, in relation to the grid it belongs in.
};

#endif
