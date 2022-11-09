#pragma once

#ifndef IMGUISLIDERFLOATPANELCONTAINER_H
#define IMGUISLIDERFLOATPANELCONTAINER_H

#include "ImGuiSliderFloatPanel.h"
#include "ImGuiWindowFeedback.h"

class ImGuiSliderFloatPanelContainer
{
	public:
		std::unordered_map<std::string, ImGuiSliderFloatPanel> panels;
		void insertNewPanel(std::string in_panelName, float in_xOffset, float in_yOffset, float in_xSize, float in_ySize)
		{
			ImGuiSliderFloatPanel newPanel(in_panelName, in_xOffset, in_yOffset, in_xSize, in_ySize);
			panels[in_panelName] = newPanel;
		};

		void insertSliderFloatIntoPanel(std::string in_panelName, std::string in_sliderFloatName, float* in_floatRef, float in_minValue, float in_maxValue)
		{
			panels[in_panelName].insertNewSliderFloat(in_sliderFloatName, in_floatRef, in_minValue, in_maxValue);
		};

		ImGuiWindowFeedback runAllSliders()
		{
			ImGuiWindowFeedback combinedFeedback;
			for (auto& currentPanel : panels)
			{
				combinedFeedback += currentPanel.second.renderSliderFloats();
			}
			return combinedFeedback;
		}
};

#endif