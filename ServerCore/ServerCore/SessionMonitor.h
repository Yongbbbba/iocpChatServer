#pragma once
#include "pch.h"

class SessionMonitor : public Work
{
public :
	SessionMonitor();
	void Tick();
};
static SessionMonitor sessionMonitor;