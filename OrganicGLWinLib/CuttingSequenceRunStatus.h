#pragma once

#ifndef CUTTINGSEQUENCERUNSTATUS_H
#define CUTTINGSEQUENCERUNSTATUS_H

#include "ErrorSensor.h"

class CuttingSequenceRunStatus
{
	public:
		bool wasRunErrorFree = true;
		bool wasTriangleDestroyed = false;
		bool sPolySurvived = true;			// would be set to false if the run was error-free AND the SPoly didn't survive
		ErrorSensor sensor;
	private:

};

#endif
