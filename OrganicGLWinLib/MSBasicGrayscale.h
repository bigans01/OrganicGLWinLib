#pragma once

#ifndef MSBASICGRAYSCALE_H
#define MSBASICGRAYSCALE_H

#include "MShaderBase.h"

class MSBasicGrayscale : public MShaderBase
{
	public:
		void setupMShaderRequestsAndName();
		Message checkRequiredBindings();
	private:
};

#endif