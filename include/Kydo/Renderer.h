#pragma once

#include <string>
#include <memory>
#include <vector>
#include <string_view>
#include "Kydo/Window.h"
#include "Kydo/Shapes.h"

namespace Kydo
{
	class KYDO_API Renderer
	{
	protected:
		Renderer() = default;
	public:
		virtual ~Renderer() = default;
		virtual void Render(std::vector<Vertex> verts, std::vector<Indice> indices) = NULL;

		virtual bool IsAlive() = NULL;

		static std::unique_ptr<Renderer> Create(Window &, std::string_view src);
	};
}