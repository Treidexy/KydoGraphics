#pragma once

#if KYDO_BUILD_DLL
#define KYDO_API __declspec(dllexport)
#elif KYDO_STATIC
#define KYDO_API
#else
#define KYDO_API __declspec(dllimport)
#endif

#define KYDO_PROPERTY(...) __declspec(property(##__VA_ARGS__))