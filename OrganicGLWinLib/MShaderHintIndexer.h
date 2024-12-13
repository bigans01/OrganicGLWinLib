#pragma once

#ifndef MSHADERHINTINDEXER_H
#define MSHADERHINTINDEXER_H

#include "MShaderHintEnum.h"

/*
* 
* Description: this class manages hints that are meant to be interpreted by the MShaderController class.
* Sets of hints can be fetched at various times by that class.

*/

class MShaderHintIndexer
{
	public:
		void insertTransitionalHint(MShaderHintEnum in_hintToAdd);
		Message processTransitionalHintMessage(Message in_hintMessage);	// processes a Message of the type MSHADER_TRANSITIONAL_HINT, 
																		// to read an MShaderHintEnum value intended for transitional operations.
		std::set<MShaderHintEnum> fetchTransitionalHints();
	private:
		std::set<MShaderHintEnum> transitionalHints;
};

#endif
