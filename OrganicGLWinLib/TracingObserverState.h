#pragma once

#ifndef TRACINGOBSERVERSTATE_H
#define TRACINGOBSERVERSTATE_H

enum class TracingObserverState
{
	CONTINUE_OBSERVE,
	FINAL_OBSERVE,
	LINE_OF_SIGHT_BROKEN,
	FINISHED
};

#endif
