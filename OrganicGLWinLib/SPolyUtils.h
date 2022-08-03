#pragma once

#ifndef SPOLYUTILS_H
#define SPOLYUTILS_H

#include "Message.h"
#include "SPolySupergroup.h"
#include "IndependentUtils.h"
#include <vector>
#include "OrganicWrappedBBFan.h"

class SPolyUtils
{
	public:
		static Message meltSPolySupergroupIntoMessage(SPolySupergroup* in_sPolySupergroupRef);
		static SPolySupergroup solidifySupergroupFromMessage(Message* in_messageRef);
		static std::vector<OrganicWrappedBBFan> produceFansFromSupergroup(SPolySupergroup* in_sPolySupergroupRef);
};

#endif
