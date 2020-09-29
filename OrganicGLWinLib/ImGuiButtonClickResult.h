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
		bool wasAButtonClicked = false;		// false by default.
};

#endif
