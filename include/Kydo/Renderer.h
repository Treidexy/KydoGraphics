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
		cl::Buffer pixelMem;
		cl::Program prog;
		cl::Kernel kernel;

		std::string src;
	public:
		Renderer(Window &, const std::string &src = {});
		~Renderer();
	};
}