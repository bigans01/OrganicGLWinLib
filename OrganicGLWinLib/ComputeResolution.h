#pragma once

#ifndef COMPUTERESOLUTION_H
#define COMPUTERESOLUTION_H

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
