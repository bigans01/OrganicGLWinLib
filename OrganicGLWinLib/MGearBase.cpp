#include "stdafx.h"
#include "MGearBase.h"

void MGearBase::setMGearPointers(MAPIObjectManager* in_mGearObjectManagerRef,
								GLUniformRegistry* in_mGearUniformRegistryRef)
{
	mGearObjectManagerRef = in_mGearObjectManagerRef;
	mGearUniformRegistryRef = in_mGearUniformRegistryRef;
}