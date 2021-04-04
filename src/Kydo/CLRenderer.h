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
		cl::Buffer vertMem;
		cl::Buffer indiceMem;
		cl::Buffer calculatedMem;

		bool alive = true;
		Window *wnd;
		std::string src;
		std::vector<std::vector<Vertex>> verts;
		std::vector<std::vector<Indice>> indices;
	public:
		CLRenderer(Window &, std::string_view src);
		~CLRenderer() override;

		void Clear(COLORREF);
		void Draw();
		virtual void Render(std::vector<Vertex> verts, std::vector<Indice> indices) override;

		bool IsDrawing();
		virtual bool IsAlive() override;
	};
}