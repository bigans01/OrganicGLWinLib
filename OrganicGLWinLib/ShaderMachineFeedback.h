#pragma once

#ifndef SHADERMACHINEFEEDBACK_H
#define SHADERMACHINEFEEDBACK_H

#include <vector>
#include <string>
#include "ImGuiInputTextResponse.h"
#include "ImGuiButtonClickResult.h"
#include "ShaderMachineFeedbackType.h"
#include "ImGuiWindowFeedback.h"

class ShaderMachineFeedback
{
	public:
		ShaderMachineFeedback() {};
		ShaderMachineFeedback& operator+=(const ShaderMachineFeedback& in_feedbackB)
		{
			// append strings
			for (auto& currentStringFeedback : in_feedbackB.feedbackStrings)
			{
				feedbackStrings.push_back(currentStringFeedback);
			}

			// append mouse feedback
			for (auto& currentMouseFeedback : in_feedbackB.mouseFeedback)
			{
				mouseFeedback[currentMouseFeedback.first] = currentMouseFeedback.second;
			}

			// append mouse window flag
			if (in_feedbackB.wasMouseInWindow == true)
			{
				wasMouseInWindow = true;
				mouseHoveredPanelName = in_feedbackB.mouseHoveredPanelName;
			}

			// append the focused window, if it was set in in_feedbackB, but not here.
			if
			(
				(wasAWindowFocused == false)
				&&
				(in_feedbackB.wasAWindowFocused == true)
			)
			{
				wasAWindowFocused = true;
				focusedWindowName = in_feedbackB.focusedWindowName;
			}

			// NOTE: ImGuiButtonClickResult isn't appended; it is manually set by the
			// call to ShaderMachineBase::checkForClickedButtons().
			wasInputTextModified = in_feedbackB.wasInputTextModified;
			return *this;
		}

		ShaderMachineFeedback& operator+=(const ImGuiInputTextResponse& in_textResponse)
		{
			feedbackStrings.push_back(in_textResponse.stringedArray);
			return *this;
		}

		ShaderMachineFeedback& operator+=(const ImGuiButtonClickResult& in_buttonClickResult)
		{
			// go ahead and copy the button click results.
			buttonClickResult = in_buttonClickResult;

			// check for hovering
			if (in_buttonClickResult.wasWindowHovered == true)
			{
				wasMouseInWindow = true;
				mouseHoveredPanelName = in_buttonClickResult.buttonPanelName;
			}

			// check for focus and set it, but only if we don't have a focus set already;
			// the focusedWindowName would be the same as the buttonPanelName, but we must set the focused window data.
			if
			(
				(wasAWindowFocused == false)
				&&
				(in_buttonClickResult.wasWindowOfButtonFocused == true)
			)
			{
				wasAWindowFocused = true;
				focusedWindowName = in_buttonClickResult.buttonPanelName;
			}

			return *this;
		}

		ShaderMachineFeedback& operator+=(const ImGuiWindowFeedback& in_windowFeedback)
		{
			// check for hovering
			if (in_windowFeedback.windowHovered == true)
			{
				wasMouseInWindow = true;
				mouseHoveredPanelName = in_windowFeedback.windowHoveredName;
			}

			// check for focus and set it, but only if we don't have a focus set already.
			if
			(
				(wasAWindowFocused == false)
				&&
				(in_windowFeedback.windowFocused == true)
			)
			{
				wasAWindowFocused = true;
				focusedWindowName = in_windowFeedback.windowFocusedName;
			}

			return *this;
		}

		void clearFeedback()
		{
			feedbackStrings.clear();
		}



		std::vector<std::string> feedbackStrings;
		std::map<ShaderMachineFeedbackType, float> mouseFeedback;
		ImGuiButtonClickResult buttonClickResult;
		std::string mouseHoveredPanelName = "";
		bool wasInputTextModified = false;		// if any input text was modified, set this value to true.
		bool wasMouseInWindow = false;			// if a mosue was ever in a ImGui panel, window, etc...this should be set to true.

		bool wasAWindowFocused = false;
		std::string focusedWindowName = "";
};

#endif