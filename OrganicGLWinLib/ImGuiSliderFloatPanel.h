#pragma once

#ifndef IMGUISLIDERFLOATPANEL_H
#define IMGUISLIDERFLOATPANEL_H

#include <unordered_map>
#include "ImGuiSliderFloat.h"
#include <string>

class ImGuiSliderFloatPanel
{
	public:
		ImGuiSliderFloatPanel()
		{
			panelName = "slider float test panel";
			xOffset = 10;
			yOffset = 120;
			xSize = 200;
			ySize = 100;
		};

		ImGuiSliderFloatPanel(std::string in_panelName, int in_xOffset, int in_yOffset, int in_xSize, int in_ySize) :
			panelName(in_panelName),
			xOffset(in_xOffset),
			yOffset(in_yOffset),
			xSize(in_xSize),
			ySize(in_ySize)
		{}

		std::unordered_map<std::string, ImGuiSliderFloat> sliderMap;
		std::string panelName = "";
		int xOffset = 0;
		int yOffset = 0;
		int xSize = 0;
		int ySize = 0;

		void renderSliderFloats()
		{
			auto slidersBegin = sliderMap.begin();
			auto slidersEnd = sliderMap.end();

			ImGui::SetNextWindowPos(ImVec2(xOffset, yOffset));
			ImGui::SetNextWindowSize(ImVec2(xSize, ySize), ImGuiSetCond_FirstUseEver);
			bool window_val = true;
			ImGui::Begin(panelName.c_str(), &window_val);

			for (; slidersBegin != slidersEnd; slidersBegin++)
			{
				slidersBegin->second.render();
			}

			ImGui::End();
		}

		void insertNewSliderFloat(std::string in_sliderName, float* in_floatRef, float in_minValue, float in_maxValue)
		{
			ImGuiSliderFloat newSlider(in_sliderName, in_floatRef, in_minValue, in_maxValue);
			sliderMap[in_sliderName] = newSlider;
		}
};

#endif
