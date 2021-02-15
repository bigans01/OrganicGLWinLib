#include "stdafx.h"
#include  "ImGuiInputText.h"

ImGuiInputTextResponse ImGuiInputText::renderAndListen()
{
	/*
	if (ImGui::InputText(inputText.c_str(), characterArray, 256))
	{
		std::cout << inputText << " was clicked! " << std::endl;
		return true;
	}
	*/

	ImGuiInputTextResponse returnResponse;
	bool wasEnterPressed = ImGui::InputText(inputText.c_str(), characterArray, 256, ImGuiInputTextFlags_EnterReturnsTrue);
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