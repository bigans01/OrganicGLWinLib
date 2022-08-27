#pragma once

#ifndef IMGUITEXTINPUT_H
#define IMGUITEXTINPUT_H

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>
#include <iostream>
#include "ImGuiInputTextResponse.h"
#include "Message.h"
#include "TrackedMap.h"
#include  <cstring>

class ImGuiInputText
{
	public:
		ImGuiInputText() {};
		ImGuiInputText(std::string in_inputText) : inputText(in_inputText) {};
		ImGuiInputTextResponse renderAndListen();		// render the InputText object with a callback, to allow for user input/modification during render time.
		std::string inputText = "";						// the string name of the InputText box.
		char characterArray[256] = { 0 };				// a zero-initialized array to store input data.
		void handleKeystrokeInputMessage(Message in_message);	// a mechanism that allows objects such as OrganicCoreLib to directly send Messages to this object, to allow 
																// it to be modified.
		int messageDepth = 0;				// how "deep" into the processedCommands we are (i.e, 3 commands deep). 
		std::string currentUserInput = "";	// the last string value the user entered, before hitting the up/down key.
		TrackedMap<int, std::string> processedCommands;	// stores commands that were attempted to be processed after hitting the enter key.
		void loadStringFromProcessedCommands(std::string in_stringToLoad);

	private:
		void clearCharacterBuffer();	// zero-out the characterArray(); needs to be called after enter key is received.
		int getZeroCount();		// used to determine the number of array elements that contain no values (i.e, 0);
								// call this function before and after ImGui::InputText to determine if changes have been made to
								// characterArray.
		static int inputTextCallback(ImGuiInputTextCallbackData* data);		// the callback used by the call to ImGui::InputText
};

#endif
