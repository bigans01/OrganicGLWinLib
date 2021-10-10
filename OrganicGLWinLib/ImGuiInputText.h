#pragma once

#ifndef IMGUITEXTINPUT_H
#define IMGUITEXTINPUT_H

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>
#include <iostream>
#include "ImGuiInputTextResponse.h"

class ImGuiInputText
{
	public:
		ImGuiInputText() {};
		ImGuiInputText(std::string in_inputText) : inputText(in_inputText) {};
		ImGuiInputTextResponse renderAndListen();
		std::string inputText = "";
		char characterArray[256] = { 0 };
	private:
		void clearCharacterBuffer();
		int getZeroCount();		// used to determine the number of array elements that contain no values (i.e, 0);
								// call this function before and after ImGui::InputText to determine if changes have been made to
								// characterArray.
};

#endif
