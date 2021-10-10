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
		template<typename FirstInput, typename ...RemainingInputs> bool doesInputCombinationExist(FirstInput && first, RemainingInputs && ...remaining)
		{
			bool doesExist = false;

			// first, get the total number of inputs that were passed into this function;
			// the number of existing inputs must match this in order to return true.
			int totalNumberOfInputParams = incrementInputCount(0, std::forward<FirstInput>(first), std::forward<RemainingInputs>(remaining)...);
			int updatedFoundInputs = getNumberOfFoundInputs(0, std::forward<FirstInput>(first), std::forward<RemainingInputs>(remaining)...);
			if (totalNumberOfInputParams == updatedFoundInputs)
			{
				//std::cout << "!!! Note, found input! " << std::endl;
				doesExist = true;
			}

			return doesExist;
		}
		void doesInputCombinationExist() {};



		void insertCycle(int in_glfwEnum)
		{
			KeyPressCycle newCycle;
			KeyPressCycleRecord newRecord(in_glfwEnum, newCycle);
			keyPressCycles.push_back(newRecord);
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
			killCycleVector.push_back(in_glfwEnum);
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
		friend class ShaderMachineBase;
		void handleKeyPressTransfers(bool in_shouldNotTransfer)
		{
			if (in_shouldNotTransfer == false)
			{
				auto cyclesPressedBegin = keyPressCycles.begin();
				auto cyclesPressedEnd = keyPressCycles.end();
				for (; cyclesPressedBegin != cyclesPressedEnd; cyclesPressedBegin++)
				{
					cycleTracker[cyclesPressedBegin->keyValue] = cyclesPressedBegin->cycle;
				}

				auto killCyclesBegin = killCycleVector.begin();
				auto killCyclesEnd = killCycleVector.end();
				for (; killCyclesBegin != killCyclesEnd; killCyclesBegin++)
				{
					auto findCycleToRelease = cycleTracker.find(*killCyclesBegin);
					if (findCycleToRelease != cycleTracker.end())
					{
						cycleTracker[*killCyclesBegin].updateAsReleased();
					}
				}

			}
			keyPressCycles.clear();	// clear out the vector, regardless of what happened (even if it was empty to begin with)
			killCycleVector.clear();
		}
		template<typename FirstInput, typename ...RemainingInputs> int incrementInputCount(int in_incrementValue, FirstInput && first, RemainingInputs && ...remaining)
		{
			int currentValue = in_incrementValue + 1;
			return incrementInputCount(currentValue, std::forward<RemainingInputs>(remaining)...);
		}
		int incrementInputCount(int in_incrementValue) { return in_incrementValue; };

		template<typename FirstInput, typename ...RemainingInputs> int getNumberOfFoundInputs(int in_incrementValue, FirstInput && first, RemainingInputs && ...remaining)
		{
			int totalFoundInputs = in_incrementValue;
			auto inputFinder = cycleTracker.find(std::forward<FirstInput>(first));
			if (inputFinder != cycleTracker.end())	// it was found.
			{
				totalFoundInputs += 1;
			}
			return getNumberOfFoundInputs(totalFoundInputs, std::forward<RemainingInputs>(remaining)...);
		}
		int getNumberOfFoundInputs(int in_incrementValue) { return in_incrementValue; };
		std::map<int, KeyPressCycle> cycleTracker;	// the int's of this map are specified by the GLFW_* enums in glfw.h.

		struct KeyPressCycleRecord
		{
			KeyPressCycleRecord() {};
			KeyPressCycleRecord(int in_keyValue, KeyPressCycle in_keyPressCycle) :
				keyValue(in_keyValue),
				cycle(in_keyPressCycle)
			{};
			int keyValue = 0;
			KeyPressCycle cycle;
		};

		std::vector<KeyPressCycleRecord> keyPressCycles;
		std::vector<int> killCycleVector;
};

#endif
