#include "get_seconds.h"

// Alec: Why is this more "hires" than chrono?
#  include <windows.h>
#  include <cassert>
double get_seconds()
{
	LARGE_INTEGER li_freq, li_current;
	const bool ret = QueryPerformanceFrequency(&li_freq);
	const bool ret2 = QueryPerformanceCounter(&li_current);
	assert(ret && ret2);
	assert(li_freq.QuadPart > 0);
	return double(li_current.QuadPart) / double(li_freq.QuadPart);
}

