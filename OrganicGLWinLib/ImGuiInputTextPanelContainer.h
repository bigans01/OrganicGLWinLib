#pragma once

#ifndef IMGUIINPUTTEXTPANELCONTAINER_H
#define IMGUIINPUTTEXTPANELCONTAINER_H


#include <string>
#include <unordered_map>
#include "ImGuiInputTextPanel.h"

class ImGuiInputTextPanelContainer
{
	public:
		std::unordered_map<std::string, ImGuiInputTextPanel> panels;
		void insertNewPanel(std::string in_panelName, float in_xOffset, float in_yOffset, float in_xSize, float in_ySize)
		{
			ImGuiInputTextPanel newPanel(in_panelName, in_xOffset, in_yOffset, in_xSize, in_ySize);
			panels[in_panelName] = newPanel;
		};

		void insertInputIntoPanel(std::string in_panelName, std::string in_buttonName)
		{
			panels[in_panelName].insertNewInput(in_buttonName);
		}

		void checkForInput()
		{
			auto panelsBegin = panels.begin();
			auto panelsEnd = panels.end();
			for (; panelsBegin != panelsEnd; panelsBegin++)
			{
				panelsBegin->second.scanForInputFocus();
			}
		};
};

#endif
