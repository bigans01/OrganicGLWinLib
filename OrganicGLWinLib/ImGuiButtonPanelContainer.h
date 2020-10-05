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
		void insertNewPanel(std::string in_panelName, int in_xOffset, int in_yOffset, int in_xSize, int in_ySize)
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

			auto panelsBegin = panels.begin();
			auto panelsEnd = panels.end();
			bool clickFound = false;
			for (; panelsBegin != panelsEnd; panelsBegin++)
			{
				ImGuiButtonClickResult result = panelsBegin->second.getClickResult();
				if	// always search for first available click.
				(
					(clickFound == false)
					&&
					(result.wasAButtonClicked == true)
				)
				{
					clickFound = true;
					returnResult = result;
				}
			}
			return returnResult;
		}

};

#endif
