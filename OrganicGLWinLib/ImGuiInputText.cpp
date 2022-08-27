#include "stdafx.h"
#include  "ImGuiInputText.h"

ImGuiInputTextResponse ImGuiInputText::renderAndListen()
{
	// Just a note: commenting out the below block of code would render the same input field twice.
	/*
	if (ImGui::InputText(inputText.c_str(), characterArray, 256))
	{
		std::cout << inputText << " was clicked! " << std::endl;
		//return true;
	}
	*/

	ImGuiInputTextResponse returnResponse;
	int preInputZeroCount = getZeroCount();

	// the last two arguments for ImGui::InputText below have the following logic:
	//	-inputTextCallBack is the static member function of this class, that has the signature (ImGuiInputTextCallbackData* data).
	//	 This callback function will have the ability to access parts of this ImGuiInputInstance, through the "this" parameter that follows it.
	//   The "this" parameter is a raw pointer that is a referenec to this instance, which is then static_cast into a ImGuiInputText pointer.
	//   Once cast, the static method can access the calling object.
	auto flagsForInput = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
	bool wasEnterPressed = ImGui::InputText(inputText.c_str(), characterArray, 256, flagsForInput, inputTextCallback, this);	
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

		// store the enteredLine in the processedCommands container.
		if (enteredLine != "")
		{
			int currentCommandSize = processedCommands.size();
			processedCommands.insert(currentCommandSize, enteredLine);
			processedCommands.setCurrentElementByKey(currentCommandSize);
		}
		messageDepth = 0;
		currentUserInput = "";

		clearCharacterBuffer();

		// set the keyboard focus to still be at this input for the next frame
		ImGui::SetKeyboardFocusHere();
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

void ImGuiInputText::handleKeystrokeInputMessage(Message in_message)
{
	
}

int ImGuiInputText::inputTextCallback(ImGuiInputTextCallbackData* data)
{
	// cast the "this" pointer used when calling this function, which is actually a pointer to the calling instance of ImGuiInputText.
	// This will allow us to access the calling instance like we normally would.
	auto convertedData = static_cast<ImGuiInputText*>(data->UserData);
	if (convertedData->processedCommands.size() != 0)
	{

		if (data->EventKey == ImGuiKey_DownArrow)
		{
			std::cout << "(ImGuiInputText): callback testing, up arrow pressed; " << std::endl;
			std::string enteredLine(convertedData->characterArray);
			std::cout << "(ImGuiInputText): callback testing, value of currentarray is: " << enteredLine << std::endl;
			if (convertedData->messageDepth != 0)
			{
				auto nextValueIter = convertedData->processedCommands.getNextElement();
				std::cout << "(ImGuiInputText): found next command string (next newest) -> " << nextValueIter->second << std::endl;
				convertedData->messageDepth--;
				std::cout << "(ImGuiInputText): messageDepth is now: " << convertedData->messageDepth << std::endl;

				if (convertedData->messageDepth == 0)
				{
					std::cout << "(ImGuiInputText): messageDepth is back at 0; use whatever input the user had before hitting enter. " << std::endl;

					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, convertedData->currentUserInput.c_str());

					std::string otherLine(convertedData->characterArray);
					std::cout << "(ImGuiInputText): current array string is: " << otherLine << std::endl;
				}
				else
				{
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, nextValueIter->second.c_str());


					std::string otherLine(convertedData->characterArray);
					std::cout << "(ImGuiInputText): current array string is: " << otherLine << std::endl;
				}
			}
		}
		else if (data->EventKey == ImGuiKey_UpArrow)
		{
			std::cout << "(ImGuiInputText): callback testing, down arrow pressed; " << std::endl;
			std::string enteredLine(convertedData->characterArray);
			std::cout << "(ImGuiInputText): callback testing, value of currentarray is: " << enteredLine << std::endl;

			if (convertedData->messageDepth == 0)
			{
				auto latestString = convertedData->processedCommands.getCurrentElementIter();
				convertedData->messageDepth++;

				std::string currentLine(convertedData->characterArray);
				std::cout << "(ImGuiInputText): value of currentLine: " << currentLine << std::endl;
				convertedData->currentUserInput = currentLine;
				std::cout << "(ImGuiInputText): current user input has been stored as: " << convertedData->currentUserInput << std::endl;

				std::cout << "(ImGuiInputText): messageDepth is now: " << convertedData->messageDepth << std::endl;

				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, latestString->second.c_str());


				std::string enteredLine(convertedData->characterArray);
				std::cout << "(ImGuiInputText): current array string is: " << enteredLine << std::endl;

			}
			else
			{
				int preDepthChangeCurrentKey = convertedData->processedCommands.getCurrentElementIter()->first;
				auto postDepthChangeCurrentKey = convertedData->processedCommands.getPreviousElement();
				if (preDepthChangeCurrentKey != postDepthChangeCurrentKey->first)	// as long as the key changed, we can increment the messageDepth.  
																					// if not, it means there was no new key (so no need to increase the depth.)
				{
					std::cout << "(ImGuiInputText): found previous command string (next oldest) -> " << postDepthChangeCurrentKey->second << std::endl;
					convertedData->messageDepth++;
					std::cout << "(ImGuiInputText): messageDepth is now: " << convertedData->messageDepth << std::endl;

					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, postDepthChangeCurrentKey->second.c_str());


					std::string enteredLine(convertedData->characterArray);
					std::cout << "(ImGuiInputText): current array string is: " << enteredLine << std::endl;
				}
				else
				{
					std::cout << "(ImGuiInputText): no change in keys; messageDepth will not be altered. command string is -> " << postDepthChangeCurrentKey->second << std::endl;
					std::cout << "(ImGuiInputText): messageDepth is now: " << convertedData->messageDepth << std::endl;
				}
			}
		}
	}
	return 0;
}



void ImGuiInputText::loadStringFromProcessedCommands(std::string in_stringToLoad)
{
	clearCharacterBuffer();
	strcpy(characterArray, in_stringToLoad.c_str());
}