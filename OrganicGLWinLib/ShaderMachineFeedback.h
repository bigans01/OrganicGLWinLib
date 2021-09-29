#pragma once

#ifndef SHADERMACHINEFEEDBACK_H
#define SHADERMACHINEFEEDBACK_H

#include <vector>
#include <string>
#include "ImGuiInputTextResponse.h"
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

		std::vector<std::string> feedbackStrings;
		std::map<ShaderMachineFeedbackType, float> mouseFeedback;
};

#endif