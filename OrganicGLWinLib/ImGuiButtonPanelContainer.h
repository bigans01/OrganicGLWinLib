#pragma once

#ifndef IMGUIBUTTONPANELCONTAINER_H
#define IMGUIBUTTONPANELCONTAINER_H

#include <string>
#include <unordered_map>
#include "ImGuiButtonPanel.h"
#include "ImGuiButtonClickResult.h"

class ImGuiButtonPanelContainer
{
	public:
		std::unordered_map<std::string, ImGuiButtonPanel> panels;
		void insertNewPanel(std::string in_panelName, float in_xOffset, float in_yOffset, float in_xSize, float in_ySize)
		{
			ImGuiButtonPanel newPanel(in_panelName, in_xOffset, in_yOffset, in_xSize, in_ySize);
			panels[in_panelName] = newPanel;
		};

		void insertButtonIntoPanel(std::string in_panelName, std::string in_buttonName)
		{
			panels[in_panelName].insertNewButton(in_buttonName);
		}

		ImGuiButtonClickResult checkAllPanelsForClickResults()
		{
			// reset name string and panel, for this tick
			mouseInButtonPanelName = "";
			mouseInButtonPanel = false;

			ImGuiButtonClickResult returnResult;

			auto panelsBegin = panels.begin();
			auto panelsEnd = panels.end();
			mouseInButtonPanel = false;
			for (; panelsBegin != panelsEnd; panelsBegin++)
			{
				ImGuiButtonClickResult result = panelsBegin->second.getClickResult();
				if	// check if the current button panel is hovered.
				(
					(result.wasWindowHovered == true)
				)
				{
					//clickFound = true;
					returnResult = result;
					returnResult.buttonPanelName = panelsBegin->first;
					mouseInButtonPanelName = panelsBegin->first;
					mouseInButtonPanel = true;
					break;
				}
			}
			
			return returnResult;
		}

		std::string mouseInButtonPanelName = "";
		bool mouseInButtonPanel = false;	// needs to be reset to false at beginning of call to checkAllPanelsForClickResults()
};

#endif
