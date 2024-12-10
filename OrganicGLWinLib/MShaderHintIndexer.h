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

		std::set<MShaderHintEnum> fetchTransitionalHints();
	private:
		std::set<MShaderHintEnum> transitionalHints;
};

#endif
