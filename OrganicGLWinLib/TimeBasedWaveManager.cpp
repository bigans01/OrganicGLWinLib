#include "stdafx.h"
#include "TimeBasedWaveManager.h"

void TimeBasedWaveManager::createTimeBasedWave(std::string in_waveName, TimeBasedWaveType in_timeBasedWaveType)
{
	int nextIndexToUse = tbwMap.getNextAvailableKey();
	switch (in_timeBasedWaveType)
	{
		case (TimeBasedWaveType::SIN): { tbwMap[nextIndexToUse].reset(new SinTBW()); tbwMap[nextIndexToUse]->initialize(); }
	}
	tbwNameLookup[in_waveName] = nextIndexToUse;
}

void TimeBasedWaveManager::updateWaveValuesInRegistry(GLUniformRegistry* in_glUniformRegistryRef)
{
	auto nameLookupBegin = tbwNameLookup.begin();
	auto nameLookupEnd = tbwNameLookup.end();
	for (; nameLookupBegin != nameLookupEnd; nameLookupBegin++)
	{
		int targetIndex = nameLookupBegin->second;
		float updatedWaveY = tbwMap[targetIndex]->calculateWaveYValue();
		(*in_glUniformRegistryRef).insertFloat(nameLookupBegin->first, updatedWaveY);
	}
}