#pragma once

#include "Kydo/Renderer.h"
#include <CL/cl2.hpp>

namespace Kydo
{
	class KYDO_API CLRenderer: public Renderer
	{
	private:
		cl::Platform platform;
		cl::Device device;
		cl::Context context;
		cl::CommandQueue q;
		cl::Buffer pixelMem;
		cl::Program prog;
		cl::Kernel clearKernel;
		cl::Kernel drawKernel;
		cl::Buffer triMem;

		bool alive = true;
		Window *wnd;
		std::string src;
		std::vector<Triangle> tris;
	public:
		CLRenderer(Window &, std::string_view src);
		~CLRenderer() override;

		void Clear(COLORREF);
		void Draw();
		virtual void Render(const Triangle &) override;

		bool IsDrawing();
		virtual bool IsAlive() override;
	};
}