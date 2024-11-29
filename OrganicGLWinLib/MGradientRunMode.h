#pragma once

#ifndef MGRADIENTRUNMODE_H
#define MGRADIENTRUNMODE_H

/*

Description: This enum class is meant to use as a flag in classes or structs that use a MGradientBase-derived child, to signal
how it should run.

*/

enum class MGradientRunMode
{
	UNSET,						// default value, needs to be set to one of the following two values.
	RUN_GRADIENT_AS_FINITE_INCREMENTING,	// intended to signal that the Gradient should stop after a specified amount of time, which should cause it to quit.
	RUN_GRADIENT_AS_CYCLICAL	// intended to signal that the Gradient should run indefinitely, until explicilty told to quit.
};

#endif
