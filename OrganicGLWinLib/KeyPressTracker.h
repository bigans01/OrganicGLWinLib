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
			//int currentCount = 1;
			//int totalNumberOfInputParams = incrementInputCount(currentCount, std::forward<RemainingInputs>(remaining)...);
			int totalNumberOfInputParams = incrementInputCount(0, std::forward<FirstInput>(first), std::forward<RemainingInputs>(remaining)...);

			// now, get the actual number of existing inputs;
			// check the first value to see if it exist here, then call the recursive function, getNumberOfFoundInputs for the rest.
			
			/*
			int currentFoundInputs = 0;
			auto inputFinder = cycleTracker.find(std::forward<FirstInput>(first));
			if (inputFinder != cycleTracker.end())	// it was found.
			{
				currentFoundInputs += 1;
			}
			int updatedFoundInputs = getNumberOfFoundInputs(currentFoundInputs, std::forward<RemainingInputs>(remaining)...);
			*/
			
			//int currentFoundInputs = 0;
			int updatedFoundInputs = getNumberOfFoundInputs(0, std::forward<FirstInput>(first), std::forward<RemainingInputs>(remaining)...);

			//std::cout << "|> totalNumberofInputParams: " << totalNumberOfInputParams << " :||: updateFoundInputs: " << updatedFoundInputs << std::endl;

			if (totalNumberOfInputParams == updatedFoundInputs)
			{
				std::cout << "!!! Note, found input! " << std::endl;
				doesExist = true;
			}

			return doesExist;
		}
		void doesInputCombinationExist() {};



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
};

#endif
