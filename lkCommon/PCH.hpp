#pragma once

#ifdef WIN32
// WinAPI & other Windows internals
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#elif defined(__linux__) || defined(__LINUX__)
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#error "Target platform not defined"
#endif

// C library
#include <cassert>

// STL
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <unordered_map>
#include <map>
#include <array>
#include <numeric>
#include <random>
