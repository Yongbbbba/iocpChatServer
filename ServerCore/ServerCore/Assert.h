#pragma once
#include "pch.h"

#undef ASSERT
#define ASSERT(x)	            Assert(x, _W(#x), _W(__FILE__), __LINE__) // not ANSI, but UNICODE

void Assert(int condition, LPCWSTR conditionStr, LPCWSTR fileName, int lineNo);