#pragma once

#ifndef MACHINEACCESSPROXY_H
#define MACHINEACCESSPROXY_H

#include "SmartIntMap.h"
#include <mutex>

class ShaderMachineBase;
class Gear;
class MachineAccessProxy
{
	public:
		MachineAccessProxy() {};
		void setMachinePtr(ShaderMachineBase* in_machinePtr);
		ShaderMachineBase* machinePtr = nullptr;

		SmartIntMap<std::unique_ptr<Gear>>* getMachineGearRef();
};

#endif
