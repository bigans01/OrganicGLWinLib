#include "stdafx.h"
#include "OpenGLFeedbackListener.h"

void OpenGLFeedbackListener::initializeFeedbackListener(KeyPressTracker* in_keyPressTrackerRef, ShaderMachineFeedback* in_machineFeedbackRef)
{
	keyPressTrackerRef = in_keyPressTrackerRef;
	machineFeedbackRef = in_machineFeedbackRef;
}

void OpenGLFeedbackListener::scanForTriggerFiringConditions()
{
	// Only bother checking if there was a key state change.
	if (keyPressTrackerRef->getKeyStateChangeValue() == true)
	{
		std::cout << "!!!!! Scanning for conditions..." << std::endl;

		for (auto& currentAvailableTrigger : availableTriggers)
		{
			// Only bother attempting to insert the trigger if it doesn't exist already
			auto existingConditionFinder = triggersInPlay.find(currentAvailableTrigger.first);
			if ((existingConditionFinder == triggersInPlay.end()))
			{
				// Check the GUI interaction condition, to determine what to do.
				switch (currentAvailableTrigger.second.fetchGUICondition())
				{
					// Condition: not dependent on GUI; in this case, just check if the input combination satisfying the trigger creation exists.
					case GUIInteractionCondition::NOT_DEPENDENT:
					{
						if (keyPressTrackerRef->doesInputCombinationExist(currentAvailableTrigger.second.fetchRequiredEnums()))
						{ 
							std::cout << "Found GUI-independent trigger firing condition for the trigger with name, " << currentAvailableTrigger.first << std::endl;
							triggersInPlay[currentAvailableTrigger.first] = currentAvailableTrigger.second;
							firedTriggerMessages.push(currentAvailableTrigger.second.fetchTriggerMessage());
						}
						break;
					}

					// Untested Condition: dependent on GUI, in that the mouse was in/hovering over a GUI window.
					case GUIInteractionCondition::IN_GUI_WINDOW:
					{
						if 
						(
							(keyPressTrackerRef->doesInputCombinationExist(currentAvailableTrigger.second.fetchRequiredEnums()))
							&&
							(machineFeedbackRef->wasMouseInImGuiObject == true)
						)
						{
							std::cout << "Found GUI-dependent (mouse is hovering/in window) trigger firing condition for the trigger with name, " << currentAvailableTrigger.first << std::endl;
							triggersInPlay[currentAvailableTrigger.first] = currentAvailableTrigger.second;
							firedTriggerMessages.push(currentAvailableTrigger.second.fetchTriggerMessage());
						}
						break;
					}

					// Untested condition: dependent on GUI, in that it should only fire when the mouse is not over a GUI window.
					case GUIInteractionCondition::NOT_IN_GUI_WINDOW:
					{
						if 
						(
							(keyPressTrackerRef->doesInputCombinationExist(currentAvailableTrigger.second.fetchRequiredEnums()))
							&&
							(machineFeedbackRef->wasMouseInImGuiObject == false)
						)
						{
							std::cout << "Found GUI-dependent (mouse is NOT hovering/in window) trigger firing condition for the trigger with name, " << currentAvailableTrigger.first << std::endl;
							triggersInPlay[currentAvailableTrigger.first] = currentAvailableTrigger.second;
							firedTriggerMessages.push(currentAvailableTrigger.second.fetchTriggerMessage());
						}
						break;
					}
				}

			}
		}
	}
}

void OpenGLFeedbackListener::scanForTriggerRemovalConditions()
{
	// Only bother checking if there was a key state change.
	if (keyPressTrackerRef->getKeyStateChangeValue() == true)
	{
		std::cout << ":: key state change detected, scanning for removals..." << std::endl;

		std::vector<std::string> triggersToRemove;
		for (auto& currentTriggerInPlay : triggersInPlay)
		{
			auto requiredVector = currentTriggerInPlay.second.fetchRequiredEnums();

			// Below: check whether or not the current keystrokes for a given trigger are still pressed on the keyboard; if they are not, release the trigger
			// and flag it for deletion
			bool isTriggerStillValid = keyPressTrackerRef->doesInputCombinationExist(requiredVector);
			if (isTriggerStillValid == false)
			{
				std::cout << "Found trigger to remove, with name: " << currentTriggerInPlay.first << std::endl;
				triggersToRemove.push_back(currentTriggerInPlay.first);
			}
		}

		for (auto& currentTriggerToRemove : triggersToRemove)
		{
			triggersInPlay.erase(currentTriggerToRemove);
		}
	}
}

std::queue<Message> OpenGLFeedbackListener::fetchTriggerMessages()
{
	return firedTriggerMessages;
}

std::queue<Message> OpenGLFeedbackListener::fetchGUIMessages()
{
	return guiFeedbackMessages;
}

void OpenGLFeedbackListener::clearFeedbackMessages()
{
	while (!firedTriggerMessages.empty())
	{
		firedTriggerMessages.pop();
	}

	while (!guiFeedbackMessages.empty())
	{
		guiFeedbackMessages.pop();
	}
}