#pragma once

#ifndef MBINDINGMAP_H
#define MBINDINGMAP_H

class MBindingMap
{
	public:
		MBindingMap() {};
		bool doesBindingExist(std::string in_bindingToFind);	// returns true if the given binding was found; will not
																		// create a new binding in the map, though.
		void insertBinding(std::string in_bindingName, int in_bindingValue);	// insert a new binding; this will auto overwrite an existing binding 
																				// (be careful)
	private:
		std::unordered_map<std::string, int> bindings;
};

#endif
