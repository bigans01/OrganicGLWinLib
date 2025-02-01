#pragma once

#ifndef COMPUTERESOLUTION_H
#define COMPUTERESOLUTION_H

/*
* 
* Description: This simple class takes in a resolution, and divides it by the compute group width, and casts it to an int; it is then
* multiplied by same divisor to get a value that has no remainder.
* 
* For example, the resolution height value of 1610, when divided by a compute dim value of 16, results in a float value of 100.625, which becomes 
* 100 when casted to an int. Multiplying 100 by 16 results in 1600.

*/

class ComputeResolution
{
	public:
		ComputeResolution(int in_width, int in_height, int in_computeGroupWidth, int in_computeGroupHeight)
		{
			computeScreenWidth = int(in_width / in_computeGroupWidth) * in_computeGroupWidth;
			computeScreenHeight = int(in_height / in_computeGroupHeight) * in_computeGroupHeight;
		};
		int computeScreenWidth, computeScreenHeight;
};

#endif
