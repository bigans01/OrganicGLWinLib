#pragma once

#ifndef TIMEBASEDWAVEMANAGER_H
#define TIMEBASEDWAVEMANAGER_H

#include "SmartIntMap.h"
#include "TimeBasedWaveBase.h"
#include "TimeBasedWaveType.h"
#include "SinTBW.h"
#include "GLUniformRegistry.h"

class TimeBasedWaveManager
{
	public:
		void createTimeBasedWave(std::string in_waveName, TimeBasedWaveType in_timeBasedWaveType);
		void updateWaveValuesInRegistry(GLUniformRegistry* in_glUniformRegistryRef);
	private:
		//std::map<int, TimeBasedWaveBase> tbwMap;
		SmartIntMap<std::unique_ptr<TimeBasedWaveBase>> tbwMap;
		std::unordered_map<std::string, int> tbwNameLookup;
};

#endif
