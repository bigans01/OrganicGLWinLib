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
			ImGuiButtonClickResult returnResult;
			for (auto& currentPanel : panels)
			{
				ImGuiButtonClickResult result = currentPanel.second.getClickResult();

				// check if the current button panel is hovered, regardless of whether or not we clicked a button;
				// this is because we need to check for panel hovering/focus.
				if	
				(
					(result.wasWindowHovered == true)
					||
					(result.wasWindowOfButtonFocused == true)
				)
				{
					returnResult = result;
					returnResult.buttonPanelName = currentPanel.first;	// store the name of the panel that the butto was in
					break;
				}
			}
			
			return returnResult;
		}
};

#endif
