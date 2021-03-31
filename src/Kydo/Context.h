#pragma once

#include "Kydo/Core.h"
#include "Kydo/Window.h"

#include <CL/cl2.hpp>

namespace Kydo
{
	class KYDO_API Program;

	class KYDO_API Context
	{
	private:
		Window *wnd;
		COLORREF *pixels;
		std::vector<Program> progams;

		cl::Context context;
		cl::CommandQueue q;

		bool destroyed = false;
	public:
		Context(Window *);
		~Context();

		cl::Context &GetContext();

		void Destroy();
	};
}

#include "Kydo/Program.h"