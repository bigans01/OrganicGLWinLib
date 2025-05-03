#pragma once

#ifndef MAPIOBJECTUPDATEENUM_H
#define MAPIOBJECTUPDATEENUM_H

/*
* 
* Description: this enum class is used with the MAPIObjectUpdate class, to determine the type of update the 
* MAPIObjectUpdate represents.

*/

enum class MAPIObjectUpdateEnum
{
	MAPI_OBJECT_CREATED,
	MAPI_OBJECT_DELETED
};

#endif
