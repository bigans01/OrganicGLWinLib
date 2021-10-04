#pragma once

#ifndef KEYPRESSCYCLE_H
#define KEYPRESSCYCLE_H

#include "KeyPressState.h"

class KeyPressCycle
{
	public:
		KeyPressCycle()
		{
			currentPressState = KeyPressState::NEWLY_PRESSED;	// always set to newly pressed upon construction.
		};

		void updateAsStillPressed()
		{
			currentPressState = KeyPressState::STILL_PRESSED;
		}

		void updateAsReleased()
		{
			currentPressState = KeyPressState::RELEASED;
		}

		KeyPressState getCurrentState()
		{
			return currentPressState;
		}

	private:
		KeyPressState currentPressState = KeyPressState::NOVAL;	// set to PRESSED upon class instantiation.
};

#endif
