#pragma once

#include "Kydo/Renderer.h"
#include <CL/cl2.hpp>

namespace Kydo
{
	class KYDO_API CLRenderer: public Renderer
	{
	private:
		bool draw;

		cl::Platform platform;
		cl::Device device;
		cl::Context context;
		cl::CommandQueue q;
		cl::Buffer pixelMem;
		cl::Program prog;
		cl::Kernel kernel;

		bool alive = true;
		Window *wnd;
		std::string src;
	public:
		CLRenderer(Window &, std::string_view src);
		void Draw();
		virtual void Render() override;

		bool IsDrawing();
		virtual bool IsAlive() override;
	};
}