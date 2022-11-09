#pragma once

#ifndef BORDERLINELINK_H
#define BORDERLINELINK_H

#include "SPolyBorderLines.h"
#include "BorderLineLinkPointState.h"

class BorderLineLink
{
	public:
		BorderLineLink() {};
		BorderLineLink(glm::vec3 in_point, SPolyBorderLines* in_linkedBorderLineA, SPolyBorderLines* in_linkedBorderLineB)
		{
			linkPoint = in_point;
			linkedBorderLines[0] = in_linkedBorderLineA;
			linkedBorderLines[1] = in_linkedBorderLineB;
		};
	private:
		friend class BorderLineLinkContainer;
		friend class SPoly;
		friend class MassZonePointClipper;
		glm::vec3 linkPoint;
		BorderLineLinkPointState linkPointState = BorderLineLinkPointState::NOVAL;	// if this is ever set to OUTSIDE_OF_MASS, the SPoly is immediately disqualified.
		SPolyBorderLines* linkedBorderLines[2];
};

#endif
