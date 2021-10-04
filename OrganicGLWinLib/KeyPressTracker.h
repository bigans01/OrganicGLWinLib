#pragma once

#ifndef KEYPRESSTRACKER_H
#define KEYPRESSTRACKER_H

#include "KeyPressCycle.h"
#include "KeyPressState.h"
#include <map>
#include <vector>
#include <GLFW/glfw3.h>
#include <mutex>
#include <iostream>

class KeyPressTracker
{
	public:
		void insertCycle(int in_glfwEnum)
		{
			KeyPressCycle newCycle;
			cycleTracker[in_glfwEnum] = newCycle;
		}

		void flagStillPressedStates()
		{
			auto existingCyclesBegin = cycleTracker.begin();
			auto existingCyclesEnd = cycleTracker.end();
			for (; existingCyclesBegin != existingCyclesEnd; existingCyclesBegin++)
			{
				if (existingCyclesBegin->second.getCurrentState() == KeyPressState::NEWLY_PRESSED)
				{
					existingCyclesBegin->second.updateAsStillPressed();
				}
			}
		}

		void killCycle(int in_glfwEnum)
		{
			cycleTracker[in_glfwEnum].updateAsReleased();
		}

		void destroyCyclesAtEndOfLife()
		{
			std::vector<int> destroyableKeyVector;
			auto cyclesBegin = cycleTracker.begin();
			auto cyclesEnd = cycleTracker.end();
			for (; cyclesBegin != cyclesEnd; cyclesBegin++)
			{
				if (cyclesBegin->second.getCurrentState() == KeyPressState::RELEASED)
				{
					destroyableKeyVector.push_back(cyclesBegin->first);
				}
			}

			auto destroyablesBegin = destroyableKeyVector.begin();
			auto destroyablesEnd = destroyableKeyVector.end();
			for (; destroyablesBegin != destroyablesEnd; destroyablesBegin++)
			{
				cycleTracker.erase(*destroyablesBegin);
			}
		}

		void printCycles()
		{
			if (!cycleTracker.empty())
			{
				auto cyclesBegin = cycleTracker.begin();
				auto cyclesEnd = cycleTracker.end();
				for (; cyclesBegin != cyclesEnd; cyclesBegin++)
				{
					std::cout << cyclesBegin->first << ", ";
				}
				std::cout << std::endl;
			}
		}

	private:
		std::map<int, KeyPressCycle> cycleTracker;	// the int's of this map are specified by the GLFW_* enums in glfw.h.
};

#endif
