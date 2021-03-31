#include <cstdio>

__declspec(dllexport) void Test()
{
	std::puts("Test works!");
	(void)std::getchar();
}