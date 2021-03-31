#pragma once

#include <CL/cl2.hpp>
#include "Kydo/Window.h"

namespace Kydo
{
	class KYDO_API Renderer
	{
	private:
		Window *wnd;

		cl::Platform platform;
		cl::Device device;
		cl::Context context;
		cl::CommandQueue q;
		cl::Memory pixelMem;
		cl::Program prog;
		cl::Kernel kernel;
	public:
		
	};
}