#pragma once

#ifndef INPUTTRIGGER_H
#define INPUTTRIGGER_H

#include "GUIInteractionCondition.h"

/*

Description: The InputTrigger class is designed to store various keyboard input criteria, and a GUI window state (i.e, mouse was in ImGui window) 
that must be met as conditions for the trigger to fire. When the condition is met for it to fire, it should be inserted into a container 
(such as std::map; see OpenGLFeedbackListener) to signify that it is in "play." When it becomes in "play," the triggerResponseMessage 
should be retrieved and inserted somewhere.

An InputTrigger can be considered no longer in play, when one of the keypresses that satisified the condition are no longer valid.

*/

class InputTrigger
{
	public:
		// Below: the templated constructor function that makes the most sense for this class. Remember: all non-Message values should be ints.
		template<typename FirstEnum, typename ...RemainingEnums> InputTrigger(Message in_triggerResponseMessage, GUIInteractionCondition in_requiredGUICondition, FirstEnum&& firstE, RemainingEnums && ...remainingEs)
		{
			// Perform recursive incursion.
			insertEnumValue(std::forward<FirstEnum>(firstE), std::forward<RemainingEnums>(remainingEs)...);

			// Set the corresponding GUI condition
			requiredGUICondition = in_requiredGUICondition;

			// Lastly, set the trigger response Message.
			triggerResponseMessage = in_triggerResponseMessage;
		}
		InputTrigger() {};

		// Below: recursive insertEnumValue functions for working with the above template
		template<typename FirstEnum, typename ...RemainingEnums> void insertEnumValue(FirstEnum&& firstE, RemainingEnums && ...remainingEs)
		{
			triggerRequiredEnums.push_back(std::forward<FirstEnum>(firstE));
			insertEnumValue(std::forward<RemainingEnums>(remainingEs)...);
		}
		void insertEnumValue() {};

		std::vector<int> fetchRequiredEnums();			// get all the required keyboard enum values required for this trigger to fire
		GUIInteractionCondition fetchGUICondition();	// get the GUI condition that must be met for this trigger to fire
		Message fetchTriggerMessage();	// once fired, or "in play," this function can be called to get the trigger Message produced.
	private:
		std::vector<int> triggerRequiredEnums;		// stores the required enum values for the trigger to fire.
		GUIInteractionCondition requiredGUICondition = GUIInteractionCondition::NOT_DEPENDENT;	// stores the required GUI condition for the trigger to fire.
		Message triggerResponseMessage;	// response Message used when trigger actually fires; store this somewhere when its returned by fetchTriggerMessage()

};

#endif
