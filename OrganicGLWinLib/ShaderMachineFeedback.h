#pragma once

#ifndef SHADERMACHINEFEEDBACK_H
#define SHADERMACHINEFEEDBACK_H

#include <vector>
#include <string>
#include "ImGuiInputTextResponse.h"
#include "ImGuiButtonClickResult.h"
#include "ShaderMachineFeedbackType.h"

class ShaderMachineFeedback
{
	public:
		ShaderMachineFeedback() {};
		ShaderMachineFeedback& operator+=(const ShaderMachineFeedback& in_feedbackB)
		{
			// append strings
			auto feedbackBStringsBegin = in_feedbackB.feedbackStrings.begin();
			auto feedbackBStringsEnd = in_feedbackB.feedbackStrings.end();
			for (; feedbackBStringsBegin != feedbackBStringsEnd; feedbackBStringsBegin++)
			{
				feedbackStrings.push_back(*feedbackBStringsBegin);
			}

			// append mouse feedback
			auto mouseFeedbackBegin = in_feedbackB.mouseFeedback.begin();
			auto mouseFeedbackEnd = in_feedbackB.mouseFeedback.end();
			for (; mouseFeedbackBegin != mouseFeedbackEnd; mouseFeedbackBegin++)
			{
				mouseFeedback[mouseFeedbackBegin->first] = mouseFeedbackBegin->second;
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

		void clearFeedback()
		{
			feedbackStrings.clear();
		}

		void loadButtonClickResults(ImGuiButtonClickResult in_clickResult)
		{
			buttonClickResult = in_clickResult;
		}

		std::vector<std::string> feedbackStrings;
		std::map<ShaderMachineFeedbackType, float> mouseFeedback;
		ImGuiButtonClickResult buttonClickResult;
		bool wasInputTextModified = false;		// if any input text was modified, set this value to true.
};

#endif