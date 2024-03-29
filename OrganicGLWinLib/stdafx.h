// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

// Some important notes on stdafx.h:
//
// -the order of includes DOES matter. (i.e, BoundaryOrientation.h must come before BoundaryPolyIndicator.h, because 
//	BoundaryPolyIndicator requires BoundaryOrientation)
//
// -given the previous point, we should always start with the standard C++ STL headers, followed by ascending order of 
//	dependency level -- i.e, level 0 is the OrganicIndependentsLib, level 1 is OrganicGLWinLib, OrganicPolyOperationsLib, OrganicWindowsAdapter
//	and level 2 is OrganicCoreLib, etc.

#pragma once

#include "targetver.h"

// Standard C++ headers
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <stack>
#include <queue>

// -------------------------------Dependency Level 0 Headers
// Custom headers, from OrganicIndependents.lib
#include "ECBPolyPoint.h"				// cleaned up, in this library. 
#include "DoublePoint.h"
#include "TriangleMaterial.h"
#include "PerfectClampEnum.h"
#include "ECBPolyPointPair.h"			// no instances of this, in this library (11/8/2022)
#include "EnclaveKeyDef.h"				// cleaned up, in this library. 
#include "BoundaryOrientation.h"		// cleaned up, in this library. 
#include "BoundaryPolyIndicator.h"		// cleaned up, in this library. 
#include "UniquePointContainer.h"		// no instances of this, in this library (11/8/2022)
#include "Message.h"					// cleaned up, in this library. 
#include "OperableIntSet.h"				// cleaned up, in this library. 
#include "IndependentUtils.h"			// cleaned up, in this library. 
#include "FRayCasterTypeEnum.h"			// no instances of this, in this library (11/8/2022)

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// reference additional headers your program requires here
