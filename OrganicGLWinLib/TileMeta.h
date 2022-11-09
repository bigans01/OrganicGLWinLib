#pragma once

#ifndef TILEMETA_H
#define TILEMETA_H


class TileMeta
{
	public:
		int x_location = 0;				// the x axis location of the AtlasTile to point to
		int y_location = 0;				// the y axis location of the AtlasTile to point to
		std::string file_location = "";  // path of the file
};

#endif
