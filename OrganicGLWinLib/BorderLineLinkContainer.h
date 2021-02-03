#pragma once

#ifndef BORDERLINELINKCONTAINER_H
#define BORDERLINELINKCONTAINER_H

#include "BorderLineLink.h"
#include <map>

class BorderLineLinkContainer
{
	public:
		void insertBorderLineLink(BorderLineLink in_borderLineLink)
		{
			int linkMapSize = int(linkMap.size());
			linkMap[linkMapSize] = in_borderLineLink;
		}
		std::map<int, BorderLineLink> linkMap;
		void updateLinkPointStatus(glm::vec3 in_pointToFind, BorderLineLinkPointState in_state)
		{
			auto linkMapBegin = linkMap.begin();
			auto linkMapEnd = linkMap.end();
			int targetIndex = 0;
			for (; linkMapBegin != linkMapEnd; linkMapBegin++)
			{
				if (linkMapBegin->second.linkPoint == in_pointToFind)
				{
					std::cout << "!! Updating point state! " << std::endl;
					linkMapBegin->second.linkPointState = in_state;
				}
			}
		}
		bool checkIfAllLinksPointsAreCoplanar()
		{
			bool areAllCoplanar = true;	// assume they are all coplanar.
			auto linkMapBegin = linkMap.begin();
			auto linkMapEnd = linkMap.end();
			for (; linkMapBegin != linkMapEnd; linkMapBegin++)
			{
				if (linkMapBegin->second.linkPointState != BorderLineLinkPointState::COPLANAR)
				{
					areAllCoplanar = false;
				}
			}
			return areAllCoplanar;
		}
};

#endif
