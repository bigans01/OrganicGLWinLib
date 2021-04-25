#include "stdafx.h"
#include "MachineAccessProxy.h"
#include "ShaderMachineBase.h"
#include "Gear.h"

void MachineAccessProxy::setMachinePtr(ShaderMachineBase* in_machinePtr)
{
	machinePtr = in_machinePtr;
}

SmartIntMap<std::unique_ptr<Gear>>* MachineAccessProxy::proxyGetMachineGearRef()
{
	return &machinePtr->gearTrain;
}

GLMultiDrawArrayJob MachineAccessProxy::proxyFetchDynamicMultiDrawArrayJob(std::string in_bufferName)
{
	return machinePtr->fetchDynamicMultiDrawArrayJobCopy(in_bufferName);
}