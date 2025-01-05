#include "stdafx.h"
#include "InputTrigger.h"

Message InputTrigger::fetchTriggerMessage()
{
	return triggerResponseMessage;
}

std::vector<int> InputTrigger::fetchRequiredEnums()
{
	return triggerRequiredEnums;
}

GUIInteractionCondition InputTrigger::fetchGUICondition()
{
	return requiredGUICondition;
}