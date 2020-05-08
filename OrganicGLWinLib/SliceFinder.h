#pragma once

#ifndef SLICEFINDER_H
#define SLICEFINDER_H

#include "CleaveSequence.h"


class SliceFinder
{
public:
	void findSlice(int in_polyAID, CleaveSequence* in_cleaveSequenceRef);
};

#endif
