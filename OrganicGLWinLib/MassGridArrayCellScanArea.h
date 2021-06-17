#pragma once

#ifndef MASSGRIDARRAYCELLSCANAREA_H
#define MASSGRIDARRAYCELLSCANAREA_H

#include "EnclaveKeyDef.h"

class MassGridArrayCellScanArea
{
	public:
		MassGridArrayCellScanArea() {};
		MassGridArrayCellScanArea(EnclaveKeyDef::EnclaveKey in_scanKeyA, EnclaveKeyDef::EnclaveKey in_scanKeyB) :
			scanKeyA(in_scanKeyA),
			scanKeyB(in_scanKeyB)
		{};
		EnclaveKeyDef::EnclaveKey scanKeyA;
		EnclaveKeyDef::EnclaveKey scanKeyB;
};

#endif
