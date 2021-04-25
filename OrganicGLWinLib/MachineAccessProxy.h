#pragma once

#ifndef MACHINEACCESSPROXY_H
#define MACHINEACCESSPROXY_H

#include "SmartIntMap.h"
#include <mutex>
#include "GLMultiDrawArrayJob.h"

class ShaderMachineBase;
class Gear;
class MachineAccessProxy
{
	public:
		MachineAccessProxy() {};
		void setMachinePtr(ShaderMachineBase* in_machinePtr);
		ShaderMachineBase* machinePtr = nullptr;

		SmartIntMap<std::unique_ptr<Gear>>* proxyGetMachineGearRef();
		GLMultiDrawArrayJob proxyFetchDynamicMultiDrawArrayJob(std::string in_bufferName);
};

#endif
