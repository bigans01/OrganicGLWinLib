#include "stdafx.h"
#include "MachineAccessProxy.h"
#include "ShaderMachineBase.h"
#include "Gear.h"

void MachineAccessProxy::setMachinePtr(ShaderMachineBase* in_machinePtr)
{
	machinePtr = in_machinePtr;
}

SmartIntMap<std::unique_ptr<Gear>>* MachineAccessProxy::getMachineGearRef()
{
	return &machinePtr->gearTrain;
}