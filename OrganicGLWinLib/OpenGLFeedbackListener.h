#pragma once

#ifndef OPENGLFEEDBACKLISTENER_H
#define OPENGLFEEDBACKLISTENER_H

#include "KeyPressTracker.h"
#include "InputTrigger.h"
#include "GUIInteractionCondition.h"
#include "ShaderMachineFeedback.h"

/*

Description: This class is designed to capture feedback from OpenGL keyboard inputs, or ImGui panels/objects; it is meant
to serve as a central location for this kind of feedback that can be queried at the end of an OpenGL frame being executed.

The machineFeedbackRef and keyPressTrackerRef should reference the ShaderMachineFeedback and KeyPressTracker in the owning MShaderController, 
respectively. 

*/

class OpenGLFeedbackListener
{
	public:
		// Below: should be called before anything else is (when MShaderController constructor is called)
		void initializeFeedbackListener(KeyPressTracker* in_keyPressTrackerRef, ShaderMachineFeedback* in_machineFeedbackRef);

		/* Below: template function for creating a new trigger, it requires :
		*	-the name given to the trigger (should be unique)
		*	-the trigger's response Message
		*	-the GUI interaction condition required for the trigger to fire
		*	-the keyboard enum value(s) required for the trigger to fire
		*/
		template <typename FirstEnum, typename ...RemainingEnums> void createTrigger(std::string in_triggerName, Message in_triggerResponseMessage, GUIInteractionCondition in_requiredGUICondition, FirstEnum&& firstE, RemainingEnums && ...remainingEs)
		{
			InputTrigger newTrigger(in_triggerResponseMessage, in_requiredGUICondition, std::forward<FirstEnum>(firstE), std::forward<RemainingEnums>(remainingEs)...);
			availableTriggers[in_triggerName] = newTrigger;
		}
		void scanForTriggerFiringConditions();	// attempts to detect new conditions that are valid for firing a trigger, and fetches the 
												// corresponding triggerResponseMessage to put into the firedTriggerMessages queue.

		void scanForTriggerRemovalConditions();	// scans for any triggers that are in triggersInPlay that need to be removed.

		std::queue<Message> fetchTriggerMessages();	// call this every tick in which scanning of keyboard feedback is desired.
		std::queue<Message> fetchGUIMessages();	// call this every tick in which scanning for GUI feedback is desired.

		void clearFeedbackMessages();	// clear the firedTriggerMessages and guiFeedbackMessages queues; should be called after the opportunity to gather
										// data via call sto fetchTriggerMessages() and fetchGUIMessages() has passed.
	private:
		ShaderMachineFeedback* machineFeedbackRef = nullptr;		// references to objects in the MShaderController that owns this instance
		KeyPressTracker* keyPressTrackerRef = nullptr;
		std::map<std::string, InputTrigger> availableTriggers;	// a list of available triggers that may exist; inserted/created with calls to createTrigger.
		std::map<std::string, InputTrigger> triggersInPlay;		// manipulated by the functions scanForTriggerFiringConditions and scanForTriggerRemovalConditions;
																// scanForTriggerFiringConditions will insert if the condition is met, scanForTriggerRemovalConditions 
																// will delete if any of the key presses that caused the trigger to fire are no longer being pressed.
																
		std::queue<Message> firedTriggerMessages;	// Message objects produced by triggers put into play will go here; these must be retrieved every tick, for available interpretations, and then
													// the queue should be wiped every tick as well.

		std::queue<Message> guiFeedbackMessages;	// Message objects produced as feedback from GUI objects (mouse click, input string etc) should go here
};

#endif
