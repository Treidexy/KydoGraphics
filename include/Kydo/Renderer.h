#pragma once

#include <string>
#include <memory>
#include <string_view>
#include "Kydo/Window.h"
#include "Kydo/Triangle.h"

namespace Kydo
{
	class KYDO_API Renderer
	{
	protected:
		Renderer() = default;
	public:
		virtual ~Renderer() = default;
		virtual void Render(const Triangle &) = NULL;

		virtual bool IsAlive() = NULL;

		static std::unique_ptr<Renderer> Create(Window &, std::string_view src);
	};
}