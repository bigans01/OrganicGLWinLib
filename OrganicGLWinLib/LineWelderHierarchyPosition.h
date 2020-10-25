#pragma once

#ifndef LINEWELDERHIERARCHYPOSITION_H
#define LINEWELDERHIERARCHYPOSITION_H

enum class LineWelderHierarchyPosition
{
	NOVAL,		// means that we just haven't set this value yet.
	SUB,		// means that the CleaveSequence is subservient to the super; this means that the LineWelder should continue running.
	SUPER		// means that the CleaveSequence is where the entire welding process began, and where it will also end; when the welder encounters this value,
				// it will end itself after that iteration.
};

#endif
