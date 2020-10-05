#pragma once

#ifndef IMGUIBUTTONPANEL_H
#define IMGUIBUTTONPANEL_H

#include <unordered_map>
#include "ImGuiButton.h"
#include "ImGuiButtonClickResult.h"
#include <string>

class ImGuiButtonPanel
{
	public:
		ImGuiButtonPanel() 
		{
			// set defaults; upper left portion og OpenGL rendering area:
			panelName = "Test panel";
			xOffset = 10;
			yOffset = 10;
			xSize = 200;
			ySize = 100;
		};
		ImGuiButtonPanel(std::string in_panelName, float in_xOffset, float in_yOffset, float in_xSize, float in_ySize) :
			panelName(in_panelName),
			xOffset(in_xOffset),
			yOffset(in_yOffset),
			xSize(in_xSize),
			ySize(in_ySize)
		{}


		std::unordered_map<std::string, ImGuiButton> buttonMap;
		std::string panelName = "";
		float xOffset = 0;
		float yOffset = 0;
		float xSize = 0;
		float ySize = 0;

		ImGuiButtonClickResult getClickResult()
		{
			ImGuiButtonClickResult result;
			auto buttonsBegin = buttonMap.begin();
			auto buttonsEnd = buttonMap.end();

			ImGui::SetNextWindowPos(ImVec2(xOffset, yOffset));
			ImGui::SetNextWindowSize(ImVec2(xSize, ySize), ImGuiSetCond_FirstUseEver);
			bool window_val = true;
			ImGui::Begin(panelName.c_str(), &window_val);

			for (; buttonsBegin != buttonsEnd; buttonsBegin++)
			{
				bool wasClicked = buttonsBegin->second.renderAndListen();
				if (wasClicked == true)
				{
					ImGuiButtonClickResult foundResult(buttonsBegin->second.buttonText);
					std::cout << "Button " << buttonsBegin->second.buttonText << " was pressed. " << std::endl;
					result = foundResult;
				}
			}
			ImGui::End();
			return result;
		};

		void insertNewButton(std::string in_buttonName)
		{
			ImGuiButton newButton(in_buttonName);
			buttonMap[in_buttonName] = newButton;
		}
};

#endif
