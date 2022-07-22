#pragma once

#ifndef SPOLYUTILS_H
#define SPOLYUTILS_H

#include "Message.h"
#include "SPolySupergroup.h"

class SPolyUtils
{
	public:
		static Message meltSPolySupergroupIntoMessage(SPolySupergroup* in_sPolySupergroupRef);
};

#endif
