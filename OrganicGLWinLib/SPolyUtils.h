#pragma once

#ifndef SPOLYUTILS_H
#define SPOLYUTILS_H

#include "Message.h"
#include "SPolySupergroup.h"
#include "IndependentUtils.h"

class SPolyUtils
{
	public:
		static Message meltSPolySupergroupIntoMessage(SPolySupergroup* in_sPolySupergroupRef);
		static SPolySupergroup solidifySupergroupFromMessage(Message* in_messageRef);
};

#endif
