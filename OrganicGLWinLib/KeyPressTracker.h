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
#include <unordered_set>

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
		void doesInputCombinationExist() {};	// required for above template function
		bool doesInputCombinationExist(std::vector<int> in_enums)	// functionally the same as the template function of the same name, but does logic via 
																	// a vector of int.
		{
			bool exists = true;
			auto enumsBegin = in_enums.begin();
			auto enumsEnd = in_enums.end();
			for (; enumsBegin != enumsEnd; enumsBegin++)
			{
				auto inputFinder = cycleTracker.find(*enumsBegin);
				if (inputFinder == cycleTracker.end())	// it wasn't found; set the flag to false and break.
				{
					exists = false;
					break;
				}
			}
			return exists;
		}

		// non-template public functions
		void resetChangeState();				// should be called before any attempted modification to keyPressCycles map (i.e., insertCycle, killCycle); 
												// ie., at the beginning of the OpenGL rendering loop iteration (see code in OrganicGLManager::renderReadyArrays())
		void insertCycle(int in_glfwEnum);		// attempts to insert a cycle with a particular GLFW enum value; the final decision is handled by the call to handleKeyPressTransfers
		void insertOneOffCycle(int in_glfwEnum);	// attempts to insert a cycle that is temporal, or "one-off", meaning it only lasts for that tick. An example
													// of this is mouse scroll input, which has no press/release.
		void flagStillPressedStates();			// flags any existing cycle marked as KeyPressState::NEWLY_PRESSED to KeyPressState::STILL_PRESSED
		void killCycle(int in_glfwEnum);		// attempts to kill a cycle with a particular GLFW enum value; the final decision is handled by the call to handleKeyPressTransfers
		void destroyCyclesAtEndOfLife();		// destroys any cycles flagged as KeyPressState::RELEASED; should be called at end of rendering loop iteration
		void printCycles();						// print the GLFW enum value of all entries in the cycleTracker map.
		void destroyOneoffs();			// ends any cycles and oneoffs (simply calls destroyCyclesatEndOfLife() and deleteOneOffInputs())
		bool getKeyStateChangeValue();			// returns current value of keyStateChange.

	private:
		friend class ShaderMachineBase;
		template<typename FirstInput, typename ...RemainingInputs> int incrementInputCount(int in_incrementValue, FirstInput && first, RemainingInputs && ...remaining)
		{
			int currentValue = in_incrementValue + 1;
			return incrementInputCount(currentValue, std::forward<RemainingInputs>(remaining)...);
		}
		int incrementInputCount(int in_incrementValue) { return in_incrementValue; };	// required for above template function

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
		int getNumberOfFoundInputs(int in_incrementValue) { return in_incrementValue; };	// required for above template function

		void handleKeyPressTransfers(bool in_shouldNotTransfer);
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
		bool keyStateChange = false;			// indicates whether or not there was a key state change detected; 
		std::unordered_set<int> oneOffCycles;	// contains keys any KeyPressCycles that would exist for only one tick; these would be removed 
												// during the call to destroyOneOffs(). See usage in OrganicGLManager::renderReadyArrays(), in
												// the OrganicCoreLib.
};

#endif
