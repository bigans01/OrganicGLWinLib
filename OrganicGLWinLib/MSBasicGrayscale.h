#pragma once

#ifndef MSBASICGRAYSCALE_H
#define MSBASICGRAYSCALE_H

#include "MShaderBase.h"
#include "BasicGrayscaleMG.h"

class MSBasicGrayscale : public MShaderBase
{
	public:
		void setupMShaderRequestsAndName();
		Message checkRequiredBindings();
		void prepareAndRender();
	private:
};

#endif