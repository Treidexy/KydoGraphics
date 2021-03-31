#include <cstdio>
#include <CL/cl2.hpp>

__declspec(dllexport) void Test()
{
	cl::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	cl::vector<cl::Device> devices;
	platforms.front().getDevices(CL_DEVICE_TYPE_ALL, &devices);

	std::string deviceName;
	devices.front().getInfo(CL_DEVICE_NAME, &deviceName);
	std::printf("%s", deviceName.c_str());
	(void)std::getchar();
}