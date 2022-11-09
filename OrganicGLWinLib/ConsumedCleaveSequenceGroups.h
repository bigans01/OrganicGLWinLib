#pragma once

#ifndef CONSUMEDCLEAVESEQUENCEGROUPS_H
#define CONSUMEDCLEAVESEQUENCEGROUPS_H


class ConsumedCleaveSequenceGroups
{
	public:
		std::map<int, OperableIntSet> groups;
		void insertConsumedCleaveSequence(int in_iterationIndex, int in_cleaveSequenceID)
		{
			groups[in_iterationIndex].intSet.insert(in_cleaveSequenceID);
		}
		void printGroups()
		{
			auto groupsBegin = groups.begin();
			auto groupsEnd = groups.end();
			for (; groupsBegin != groupsEnd; groupsBegin++)
			{
				std::cout << "Group ----------> " << groupsBegin->first << std::endl;
				auto currentGroupSequencesBegin = groupsBegin->second.intSet.begin();
				auto currentGroupSequencesEnd = groupsBegin->second.intSet.end();
				for (; currentGroupSequencesBegin != currentGroupSequencesEnd; currentGroupSequencesBegin++)
				{
					std::cout << "Consumed sequence: " << *currentGroupSequencesBegin << std::endl;
				}
			}
		}
};

#endif
