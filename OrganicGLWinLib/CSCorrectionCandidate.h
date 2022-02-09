#pragma once

#ifndef CSCORRECTIONCANDIDATE_H
#define CSCORRECTIONCANDIDATE_H

#include "IRPointType.h"


class CSCorrectionCandidate
{
	public:
		CSCorrectionCandidate() {};
		CSCorrectionCandidate(int in_lineID,
				int in_dimLineID,
				IRPointType in_type) :
			lineID(in_lineID),
			dimLineID(in_dimLineID),
			matchedType(in_type)
		{}
		int lineID = 0;				// the index of the CategorizedLine in its respective CleaveSequence
		int dimLineID = 0;			// the index of the 1 dim line that this CatgorizedLnie matched to 
		IRPointType matchedType = IRPointType::NEITHER;		// used to determine if it was point a or point b of the CategorizedLine, that
															// hit the 1-dim line

};

#endif
