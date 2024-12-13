#include "stdafx.h"
#include "MShaderHintIndexer.h"

Message MShaderHintIndexer::processTransitionalHintMessage(Message in_hintMessage)
{
	// Assmes that the Message is of the type MSHADER_TRANSITIONAL_HINT,
	// and there is an int value in the Message that represents the enum'ed value of MShaderHintEnum to process.
	in_hintMessage.open();
	MShaderHintEnum messageEnumValue = MShaderHintEnum(in_hintMessage.readInt());
	Message returnInfoMessage;
	switch (messageEnumValue)
	{
		case MShaderHintEnum::TRANSIT_CLEAR_COLOR:
		{
			insertTransitionalHint(messageEnumValue);
			returnInfoMessage = Message(MessageType::MSHADER_INFO, std::string("Using hint: TRANSIT_CLEAR_COLOR."));
			break;
		}
	}

	return returnInfoMessage;
}

void MShaderHintIndexer::insertTransitionalHint(MShaderHintEnum in_hintToAdd)
{
	transitionalHints.insert(in_hintToAdd);
}

std::set<MShaderHintEnum> MShaderHintIndexer::fetchTransitionalHints()
{
	return transitionalHints;
}