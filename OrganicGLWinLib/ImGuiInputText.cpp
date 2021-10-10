#include "stdafx.h"
#include  "ImGuiInputText.h"

ImGuiInputTextResponse ImGuiInputText::renderAndListen()
{
	// commenting out the below block of code would render the same input field twice.
	/*
	if (ImGui::InputText(inputText.c_str(), characterArray, 256))
	{
		std::cout << inputText << " was clicked! " << std::endl;
		//return true;
	}
	*/

	ImGuiInputTextResponse returnResponse;
	int preInputZeroCount = getZeroCount();
	bool wasEnterPressed = ImGui::InputText(inputText.c_str(), characterArray, 256, ImGuiInputTextFlags_EnterReturnsTrue);
	int postInputZeroCount = getZeroCount();

	// if these are not equal to each other, it means the characterArray was modified (it received a new input character, or deleted one via backspace.)
	if (preInputZeroCount != postInputZeroCount)
	{
		returnResponse.wasInputReceived = true;
	}

	if (wasEnterPressed == true)
	{
		//std::cout << "Enter key was pressed. " << std::endl;
		std::string enteredLine(characterArray);
		returnResponse.setResponse(enteredLine);
		clearCharacterBuffer();
	}

	return returnResponse;
}

void ImGuiInputText::clearCharacterBuffer()
{
	for (int x = 0; x < 256; x++)
	{
		characterArray[x] = 0;
	} 
}

int ImGuiInputText::getZeroCount()
{
	int zeroCount = 0;
	for (int x = 0; x < 256; x++)
	{
		if (characterArray[x] == 0)
		{
			zeroCount++;
		}
	}
	return zeroCount;
}