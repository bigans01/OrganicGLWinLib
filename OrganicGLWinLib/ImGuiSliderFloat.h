#pragma once

#ifndef IMGUISLIDERFLOAT_H
#define IMGUISLIDERFLOAT_H

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"



class ImGuiSliderFloat
{
	public:
		ImGuiSliderFloat() {};
		ImGuiSliderFloat(std::string in_sliderText, float* in_sliderValueRef, float in_minValue, float in_maxValue) :
			sliderText(in_sliderText),
			sliderValueRef(in_sliderValueRef),
			minValue(in_minValue),
			maxValue(in_maxValue) {};
		std::string sliderText = "";
		float* sliderValueRef = NULL;
		float  minValue = 0.0f;
		float  maxValue = 0.0f;

		void render()
		{
			ImGui::SliderFloat(sliderText.c_str(), sliderValueRef, minValue, maxValue);
		}
};

#endif
