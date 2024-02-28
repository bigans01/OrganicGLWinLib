#pragma once

#ifndef GPUWORLDCOORDINATE_H
#define GPUWORLDCOORDINATE_H

/*

Description: 

This class is meant to store data eventually events get sent to ShaderMachine-derived classes. The worldBPKey should be used 
in corresponding uniform values in shaders; the data in the worldBPKey can be used to represent items such as the blueprint-key
of a world camera, or other object. The worldLocalizedPoint should store the remainder of the coordinate conversion.

It should be noted that the original double value that is used to generate the data is NOT stored in this class;
it is unnecessary, as the ShaderMachine that this data is ultimately sent to will have no use for the doubles. 

THe corresponding class, GPUWorldCoordinateProducer, is what actually takes in the double values, to 
produce an instance of this class.

*/

class GPUWorldCoordinate
{
	public:
		// Below: this constructor should always be used when initially creating the data.
		GPUWorldCoordinate(EnclaveKeyDef::EnclaveKey in_blueprintKey, ECBPolyPoint in_worldLocalizedPoint) :
			worldBPKey(in_blueprintKey),
			worldLocalizedPoint(in_worldLocalizedPoint)
		{};

		GPUWorldCoordinate();	// should never be used for initialization, but is required for direct copying.

		void printCoordinate();

		EnclaveKeyDef::EnclaveKey worldBPKey;
		ECBPolyPoint worldLocalizedPoint;
	private:
};

#endif
