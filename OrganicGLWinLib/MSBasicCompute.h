#pragma once

#ifndef MSBASICCOMPUTE_H
#define MSBASICCOMPUTE_H

#include "MShaderBase.h"

class MSBasicCompute : public MShaderBase
{
	public:	
		void setupMShaderRequests();
		Message checkRequiredBindings();
	private:
};

#endif
