#pragma once

#ifndef MAPIOBJECTTYPE_H
#define MAPIOBJECTTYPE_H

/*
* 
* Description: This enum class is used in tandem with the MAPIObjectRequest class, and other functions, to map
* various API object types to a distinct enum value.

*/

enum class MAPIObjectType
{
	UNSET,		// default value
	BUFFER,		// intended for storing buffer IDs
	TEXTURE,	// " " texture IDs
	FBO,		// " " for custom frame buffer objects
	VAO			// " " VAO IDs
};

#endif
