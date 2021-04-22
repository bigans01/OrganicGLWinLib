#include "stdafx.h"
#include "SinTBW.h"

void SinTBW::initialize()
{
	startTimePoint = std::chrono::high_resolution_clock::now();
	setTimeDenominator(1000.00f);
	setFrequencyMultiplier(.5f);
}

float SinTBW::calculateWaveYValue()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto currentMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTimePoint).count();
	float currentWaveY = (sin(((currentMilliseconds / timeDenominator) * frequencyMultiplier) * pi) + 1) / 2;
	return currentWaveY;
}
