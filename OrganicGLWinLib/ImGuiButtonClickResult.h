#pragma once

#ifndef IMGUIBUTTONCLICKRESULT_H
#define IMGUIBUTTONCLICKRESULT_H

#include <iostream>

class ImGuiButtonClickResult
{
	public:
		ImGuiButtonClickResult() {};
		ImGuiButtonClickResult(std::string in_clickedName) : clickedName(in_clickedName) { wasAButtonClicked = true; };
		std::string clickedName = "";
		std::string buttonPanelName = "";	// stores the name representing a clicked button panel, if one was clicked;
											// should be set to true if wasWindowHovered gets set to true.								

		bool wasAButtonClicked = false;		// false by default.
		bool wasWindowHovered = false;		// ""

		bool wasWindowOfButtonFocused = false;
		std::string windowOfFocusedButtonName = "";
};

#endif
