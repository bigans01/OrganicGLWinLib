#pragma once

#ifndef MSBASICCOMPUTE_H
#define MSBASICCOMPUTE_H

#include "MShaderBase.h"

/*

Description: The first prototype MSShaderBase; likely to be built in a similiar
fashion to SMLocalizedLightingComputeV1.

The value set for mShaderName, for this class, is "MSBasicCompute".

*/

class MSBasicCompute : public MShaderBase
{
	public:	
		void setupMShaderRequestsAndName();
		Message checkRequiredBindings();
	private:
};

#endif
