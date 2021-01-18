#include "stdafx.h"
#include "RayIntersectionResult.h"

void RayIntersectionResult::setResult(int in_result)
{
	if (wasResultSet == 0)	// the result should only be set one time.
	{
		wasIntersectFound = in_result;
		wasResultSet = 1;
		//wasResultSet = in_result;
	}
}