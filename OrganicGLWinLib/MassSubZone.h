#pragma once

#ifndef MASSSUBZONE_H
#define MASSSUBZONE_H

#include "SPoly.h"

class MassSubZone
{
	public:
		MassSubZone() {};
		MassSubZone(SPoly in_sPolyCopy) :
			sPolyCopy(in_sPolyCopy)
		{};
		SPoly sPolyCopy;
};

#endif

