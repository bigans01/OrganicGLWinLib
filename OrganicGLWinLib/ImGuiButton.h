#pragma once

#ifndef IMGUIBUTTON_H
#define IMGUIBUTTON_H

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class ImGuiButton
{
	public:
		ImGuiButton() {};
		ImGuiButton(std::string in_buttonText) : buttonText(in_buttonText) {};
		bool renderAndListen()
		{
			if (ImGui::Button(buttonText.c_str()))
			{
				std::cout << buttonText << " was clicked! " << std::endl;
				return true;
			}
			else return false;
		}

		std::string buttonText = "";
};

#endif
