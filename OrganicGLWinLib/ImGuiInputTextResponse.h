#pragma once

#ifndef IMGUIINPUTTEXTRESPONSE_H
#define IMGUIINPUTTEXTRESPONSE_H

#include <string>

class ImGuiInputTextResponse
{
	public:
		ImGuiInputTextResponse() {};
		void setResponse(std::string in_string)
		{
			stringedArray = in_string;
			containsResponse = true;
		};
		bool containsResponse = false;
		std::string stringedArray;
};

#endif
