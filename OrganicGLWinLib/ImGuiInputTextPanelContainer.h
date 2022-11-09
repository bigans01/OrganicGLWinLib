#pragma once

#ifndef IMGUIINPUTTEXTPANELCONTAINER_H
#define IMGUIINPUTTEXTPANELCONTAINER_H


#include "ImGuiInputTextPanel.h"
#include "ShaderMachineFeedback.h"

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

		ShaderMachineFeedback checkForInput()
		{
			ShaderMachineFeedback containerFeedback;
			auto panelsBegin = panels.begin();
			auto panelsEnd = panels.end();
			for (; panelsBegin != panelsEnd; panelsBegin++)
			{
				containerFeedback += panelsBegin->second.scanForInputFocus();
			}
			return containerFeedback;
		};

		void sendInputToInputText(Message in_inputMessage)
		{
			// remember: the Message should have two strings -- one representing the name of the ImGuiInputTextPanel,
			// and the other representing the name of the ImGuiInputText object within said panel. In this function, 
			// we will open the message and read the first string to determine the panel to go to.
			in_inputMessage.open();
			auto panelName = in_inputMessage.readString();

			// check that the panel exists.
			auto panelFinder = panels.find(panelName);
			if (panelFinder != panels.end())	
			{
				// we found the panel, now call sendKeystrokeToInputTextObject on it.
				std::cout << "(ImGuiInputTextPanelContainer): found container to send input to. " << std::endl;
				panelFinder->second.sendKeystrokeToInputTextObject(std::move(in_inputMessage));
			}

		}
};

#endif
