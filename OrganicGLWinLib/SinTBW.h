#pragma once

#ifndef SINTBW_H
#define SINTBW_H

#include "TimeBasedWaveBase.h"
#include <chrono>

class SinTBW : public TimeBasedWaveBase
{
	public:
		void initialize();
		float calculateWaveYValue();
};

#endif
