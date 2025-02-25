#pragma once

#ifndef IMGUIINPUTTEXTPANEL_H
#define IMGUIINPUTTEXTPANEL_H

#include "ImGuiInputText.h"
#include "ImGuiInputTextResponse.h"
#include "ShaderMachineFeedback.h"

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


			ImGui::SetNextWindowPos(ImVec2(xOffset, yOffset));
			ImGui::SetNextWindowSize(ImVec2(xSize, ySize), ImGuiSetCond_FirstUseEver);
			bool window_val = true;

			// for in case we find a response.
			bool foundMessageResponse = false;
			std::string foundMessage = "";

			ImGui::Begin(panelName.c_str(), &window_val);

			for (auto& currentInput : inputMap)
			{
				ImGuiInputTextResponse currentPotentialResponse = currentInput.second.renderAndListen();
				if (currentPotentialResponse.containsResponse == true)
				{
					foundMessageResponse = true;
					foundMessage = currentPotentialResponse.stringedArray;
					feedback += currentPotentialResponse;
				}
				if (currentPotentialResponse.wasInputReceived == true)
				{
					feedback.wasInputTextModified = true;
				}
			}

			// need to return true if the mouse was discovered as being within
			// in this specific text panel, so that we may ignore mouse clicks 
			// meant for the OpenGL area outside of the panel.

			if (ImGui::IsWindowHovered() == true)
			{
				feedback.wasMouseInImGuiObject = true;
				feedback.mouseHoveredPanelName = panelName;
			}

			if (ImGui::IsWindowFocused() == true)
			{
				feedback.wasAWindowFocused = true;
				feedback.focusedWindowName = panelName;
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

		void sendKeystrokeToInputTextObject(Message in_messageToSend)
		{
			// remove one string from this message (the first one), to read the next one (which would be the panel name)
			in_messageToSend.removeStringsFromFrontAndResetIter(1);
			auto inputTextName = in_messageToSend.readString();

			// check that the input text exists
			auto inputTextFinder = inputMap.find(inputTextName);
			if (inputTextFinder != inputMap.end())
			{
				std::cout << "(ImGuiInputTextPanel): found input text object to send input to. " << std::endl;
				inputTextFinder->second.handleKeystrokeInputMessage(std::move(in_messageToSend));
			}
		}
};

#endif
