#pragma once

#if KYDO_STATIC
#define KYDO_API
#elif KYDO_BUILD
#define KYDO_API __declspec(dllexport)
#else
#define KYDO_API __declspec(dllimport)
#endif

#define KYDO_PROPERTY(...) __declspec(property(##__VA_ARGS__))