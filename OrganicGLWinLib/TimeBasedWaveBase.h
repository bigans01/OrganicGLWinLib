#pragma once

#ifndef TIMEBASEDWAVE_H
#define TIMEBASEDWAVE_H

#include <chrono>

class TimeBasedWaveBase
{
	public:
		virtual void initialize() = 0;
		virtual float calculateWaveYValue() = 0;

		void setTimeNumerator(float in_timeNumerator) ;
		void setTimeDenominator(float in_timeDenominator);
		void setFrequencyMultiplier(float in_frequencyMultiplier);
	protected:
		const float pi = 3.141592f;	// always remains constant
		std::chrono::steady_clock::time_point timeNumerator;				// i.e., current number of total milliseconds that elapsed
		float timeDenominator = 1.0f;			// i.e., would be 1000 for 1 second, if numerator values were in milliseconds.
		float frequencyMultiplier = 1.0f;		// smaller value = bigger wavelengths
		std::chrono::steady_clock::time_point startTimePoint;	// the start time point; should always be set by a call to initialize().
};

#endif
