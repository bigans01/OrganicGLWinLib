#pragma once

#ifndef IMGUISLIDERFLOATPANELCONTAINER_H
#define IMGUISLIDERFLOATPANELCONTAINER_H

#include <string>
#include <unordered_map>
#include "ImGuiSliderFloatPanel.h"

class ImGuiSliderFloatPanelContainer
{
	public:
		std::unordered_map<std::string, ImGuiSliderFloatPanel> panels;
		void insertNewPanel(std::string in_panelName, int in_xOffset, int in_yOffset, int in_xSize, int in_ySize)
		{
			ImGuiSliderFloatPanel newPanel(in_panelName, in_xOffset, in_yOffset, in_xSize, in_ySize);
			panels[in_panelName] = newPanel;
		};

		void insertSliderFloatIntoPanel(std::string in_panelName, std::string in_sliderFloatName, float* in_floatRef, float in_minValue, float in_maxValue)
		{
			panels[in_panelName].insertNewSliderFloat(in_sliderFloatName, in_floatRef, in_minValue, in_maxValue);
		};

		void runAllSliders()
		{
			auto panelsBegin = panels.begin();
			auto panelsEnd = panels.end();
			for (; panelsBegin != panelsEnd; panelsBegin++)
			{
				panelsBegin->second.renderSliderFloats();
			}
		}
};

#endif