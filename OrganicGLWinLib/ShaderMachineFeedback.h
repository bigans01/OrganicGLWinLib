#pragma once

#ifndef SHADERMACHINEFEEDBACK_H
#define SHADERMACHINEFEEDBACK_H

#include <vector>
#include <string>
#include "ImGuiInputTextResponse.h"

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
};

#endif