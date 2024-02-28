#pragma once

#ifndef GPUCOORDINATEMODE_H
#define GPUCOORDINATEMODE_H

/*

Description:

This enum class is used as a simple flag by ShderMachines, to show the mode that it is going to use when operating on vertex data.
Objects such as an OrganicSystem will need to know this value, so that they know if any adjustments need to be made to vertex data
before it is sent to the GPU.

*/

enum class GPUCoordinateMode
{
	COORDINATE_MODE_UNDEFINED,	// the default value that is set in ShaderMachineBase; should always be replaced by one of the other two below.

	COORDINATE_MODE_LOCAL,	// when this mode is active, the camera of the ShaderMachine should always be at 0,0,0; the corresponding gears and shaders
							// of the ShaderMachine also need to be designed to use data from a GPUWorldCoordinate object. It also means all vertex data
							// sent to the GPU needs to be adjusted in vertex shaders to re-align the points to the camera  being at 0,0,0.

	COORDINATE_MODE_ABSOLUTE,	// when using this mode, the camera's position is not necessarily at 0,0,0. User input moves the camera around,
								// and there is no special logic needed in shaders to re-align the points. However, vertices get weird when using
								// high float values (i.e, 1000+)
};

#endif
