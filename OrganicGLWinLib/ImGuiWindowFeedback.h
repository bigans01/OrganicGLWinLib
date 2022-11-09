#pragma once
#ifndef IMGUIWINDOWFEEDBACK_H
#define IMGUIWINDOWFEEDBACK_H


class ImGuiWindowFeedback
{
	public:
		ImGuiWindowFeedback() {};
		ImGuiWindowFeedback& operator+=(const ImGuiWindowFeedback& in_windowFeedbackB)
		{
			// set the focused window, if it wasn't set already.
			if
			(
				(windowFocused == false)
				&&
				(in_windowFeedbackB.windowFocused == true)
			)
			{
				setFocusedWindow(in_windowFeedbackB.windowFocusedName);
			}

			// set the hovered window, if it wasn't set already.
			if
			(
				(windowHovered == false)
				&&
				(in_windowFeedbackB.windowHovered == true)
			)
			{
				setHoveredWindow(in_windowFeedbackB.windowHoveredName);
			}
			return *this;
		}

		void setHoveredWindow(std::string in_windowHoveredName)
		{
			windowHovered = true;
			windowHoveredName = in_windowHoveredName;
		}

		void setFocusedWindow(std::string in_windowFocusedName)
		{
			windowFocused = true;
			windowFocusedName = in_windowFocusedName;
		}
		

		bool windowFocused = false;
		std::string windowFocusedName = "";
		
		bool windowHovered = false;
		std::string windowHoveredName = "";
};

#endif
