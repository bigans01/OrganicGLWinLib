#pragma once

#ifndef MGRADIENTBASE_H
#define MGRADIENTBASE_H

#include "Message.h"

class MGradientBase
{
	public:
		virtual void initializeGradient(Message in_gradientInfo) = 0;
		virtual Message calculateByTime(float in_piValue) = 0;
	protected:
		const float fullRadians = 6.28319f;	// the equivalent of 2pi
};

#endif
