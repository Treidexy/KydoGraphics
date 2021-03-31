#include "Kydo/Renderer.h"

#include "Kydo/CLRenderer.h"

namespace Kydo
{
	std::unique_ptr<Renderer> Renderer::Create(Window &wnd, const std::string_view src)
	{ return std::make_unique<CLRenderer>(wnd, src); }
}