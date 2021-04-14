#pragma once

#ifndef COPLANARFUSIONTYPE_H
#define COPLANARFUSIONTYPE_H

enum class CoplanarFusionType
{
	NOVAL,
	KEEP_OLD_APPEND_NEW,	// if group 1 (the new group, group ID 1) has fault lines (CleaveSequences):
							// -keep all SPolys of the old group, 
							// -append the fractured results of the new group, 
							// -and remove any SPolys in the new group that were consumed by the old group.

	KEEP_NEW_APPEND_OLD	    // if group 0 (the old group, group ID 0) has fault lines (CleaveSequences): 
							// -keep all SPolys of the new group 
							// -append the fractured results of the old group
							// -and remove any SPolys in the old group that were consumed by the new group.
};

#endif
