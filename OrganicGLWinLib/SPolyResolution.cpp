#include "stdafx.h"
#include "SPolyResolution.h"

void SPolyResolution::calculateResolution()
{
	// determine which set of methods to use (BLOCK, ENCLAVE, COLLECTION)
	switch (resolutionBoxType)
	{
		case MassZoneBoxType::BLOCK: { runBlockMethodSets(); break;}
		case MassZoneBoxType::ENCLAVE: { runEnclaveMethodSets(); break;}
		case MassZoneBoxType::COLLECTION: { runCollectionMethodSets(); break;}
	}
}

void SPolyResolution::runBlockMethodSets() 
{

}

void SPolyResolution::runEnclaveMethodSets()
{

}

void SPolyResolution::runCollectionMethodSets()
{

}

