#pragma once

#ifndef SELFCOMPAREPERMIT_H
#define SELFCOMPAREPERMIT_H

#include "PermitType.h"

class SelfComparePermit
{
	public:
		void setPermitInitialData(PermitType in_permType, int in_cleaveSequenceID)
		{
			permType = in_permType;
			first_permit_action  = true;
			cleaveSequenceID = in_cleaveSequenceID;
		}



		void resetPermit()
		{
			permType = PermitType::NO_VAL;
			first_permit_action = false;
		}

		PermitType permType = PermitType::NO_VAL;
		bool first_permit_action = false;
		int cleaveSequenceID = 0;
};

#endif
