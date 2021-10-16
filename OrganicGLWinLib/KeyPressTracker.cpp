#include "stdafx.h"
#include "KeyPressTracker.h"

void KeyPressTracker::insertCycle(int in_glfwEnum)
{
	KeyPressCycle newCycle;
	KeyPressCycleRecord newRecord(in_glfwEnum, newCycle);
	keyPressCycles.push_back(newRecord);
}

void KeyPressTracker::resetChangeState()
{
	keyStateChange = false;
}

void KeyPressTracker::flagStillPressedStates()
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

void KeyPressTracker::killCycle(int in_glfwEnum)
{
	killCycleVector.push_back(in_glfwEnum);
}

bool KeyPressTracker::getKeyStateChangeValue()
{
	return keyStateChange;
}

void KeyPressTracker::destroyCyclesAtEndOfLife()
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

void KeyPressTracker::printCycles()
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

void KeyPressTracker::handleKeyPressTransfers(bool in_shouldNotTransfer)
{
	if (in_shouldNotTransfer == false)
	{
		// if either the keyPressCycles -OR- the killCycleVector is non-empty, flag this instance of KeyPressTracker as being modified.
		// An insert or delete will modify the key inputs; so set the keyStateChange value appropriately.
		auto cyclesPressedBegin = keyPressCycles.begin();
		auto cyclesPressedEnd = keyPressCycles.end();
		for (; cyclesPressedBegin != cyclesPressedEnd; cyclesPressedBegin++)
		{
			cycleTracker[cyclesPressedBegin->keyValue] = cyclesPressedBegin->cycle;
			keyStateChange = true;
		}

		auto killCyclesBegin = killCycleVector.begin();
		auto killCyclesEnd = killCycleVector.end();
		for (; killCyclesBegin != killCyclesEnd; killCyclesBegin++)
		{
			auto findCycleToRelease = cycleTracker.find(*killCyclesBegin);
			if (findCycleToRelease != cycleTracker.end())
			{
				std::cout << "!!! Key press " << *killCyclesBegin << " was RELEASED; erasing..." << std::endl;
				cycleTracker[*killCyclesBegin].updateAsReleased();
				keyStateChange = true;
			}
		}

		if (keyStateChange == true)
		{
			std::cout << "!!! Key state change detected. " << std::endl;
		}

	}
	keyPressCycles.clear();	// clear out the vector, regardless of what happened (even if it was empty to begin with)
	killCycleVector.clear();
}