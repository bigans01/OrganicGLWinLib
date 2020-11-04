#pragma once

#ifndef TRACINGOBSERVERSTATE_H
#define TRACINGOBSERVERSTATE_H

enum class TracingObserverState
{
	CONTINUE_OBSERVE,
	FINAL_OBSERVE,
	TERMINATED,		// used if the line of sight is broken, or radians are invalid. Should stop the TracingObserver.
	FINISHED
};

#endif
