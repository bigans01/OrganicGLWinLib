#pragma once

#ifndef IMGUIINPUTTEXTRESPONSE_H
#define IMGUIINPUTTEXTRESPONSE_H


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
		bool wasInputReceived = false;	// set to true if the input was modified in any way.
		std::string stringedArray;
};

#endif
