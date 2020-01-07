#pragma once

#ifndef SIMPLEUNIQUE_H
#define SIMPLEUNIQUE_H

#include <mutex>

class SimpleUnique
{
public:
	SimpleUnique();
	SimpleUnique(const SimpleUnique& simple_orig);

	std::unique_ptr<int> someInt;
	SimpleUnique& operator=(const SimpleUnique& in_uniqueB)
	{
		//someInt = std::move(in_uniqueB.someInt);
		//someInt[0] = in_uniqueB.someInt[0];
		return *this;
	}

	SimpleUnique& operator=(SimpleUnique&& in_uniqueB)
	{
		someInt = std::move(in_uniqueB.someInt);
		return *this;
	}
};

#endif
