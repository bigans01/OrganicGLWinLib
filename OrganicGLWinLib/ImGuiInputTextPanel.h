#pragma once

#ifndef IMGUIINPUTTEXTPANEL_H
#define IMGUIINPUTTEXTPANEL_H

#include <unordered_map>
#include "ImGuiInputText.h"
#include "ImGuiInputTextResponse.h"
#include "ShaderMachineFeedback.h"
#include <string>

class ImGuiInputTextPanel
{
	public:
		ImGuiInputTextPanel()
		{
			panelName = "Test input panel";
			xOffset = 10;
			yOffset = 10;
			xSize = 200;
			ySize = 100;
		};
		ImGuiInputTextPanel(std::string in_panelName, float in_xOffset, float in_yOffset, float in_xSize, float in_ySize) :
			panelName(in_panelName),
			xOffset(in_xOffset),
			yOffset(in_yOffset),
			xSize(in_xSize),
			ySize(in_ySize)
		{}

		std::unordered_map<std::string, ImGuiInputText> inputMap;	// it is possible to have multiple inputs per panel. May not be really needed, but oh well.
		std::string panelName = "";
		float xOffset = 0;
		float yOffset = 0;
		float xSize = 0;
		float ySize = 0;

		ShaderMachineFeedback scanForInputFocus()
		{
			ShaderMachineFeedback feedback;

			auto inputMapBegin = inputMap.begin();
			auto inputMapEnd = inputMap.end();

			ImGui::SetNextWindowPos(ImVec2(xOffset, yOffset));
			ImGui::SetNextWindowSize(ImVec2(xSize, ySize), ImGuiSetCond_FirstUseEver);
			bool window_val = true;

			// for in case we find a response.
			bool foundMessageResponse = false;
			std::string foundMessage = "";

			ImGui::Begin(panelName.c_str(), &window_val);

			for (; inputMapBegin != inputMapEnd; inputMapBegin++)
			{
				ImGuiInputTextResponse currentPotentialResponse = inputMapBegin->second.renderAndListen();
				if (currentPotentialResponse.containsResponse == true)
				{
					foundMessageResponse = true;
					foundMessage = currentPotentialResponse.stringedArray;
					feedback += currentPotentialResponse;
				}
			}

			ImGui::End();

			if (foundMessageResponse == true)
			{
				std::cout << "Produced string: " << foundMessage << std::endl;
			}

			return feedback;
		}

		void insertNewInput(std::string in_inputName)
		{
			ImGuiInputText newInput(in_inputName);
			inputMap[in_inputName] = newInput;
		}
};

#endif
