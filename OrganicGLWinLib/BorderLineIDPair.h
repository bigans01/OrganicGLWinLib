#pragma once

#ifndef BORDERLINEIDPAIR_H
#define BORDERLINEIDPAIR_H

class BorderLineIDPair
{
	public:
		BorderLineIDPair(int in_pointABorderLineID, int  in_pointBBorderLineID) : pointABorderLineID(in_pointABorderLineID), pointBBorderLineID(in_pointBBorderLineID) {};
		int pointABorderLineID, pointBBorderLineID;
};

#endif
