#pragma once

#include "Kydo/Core.h"

namespace Kydo
{
	using Indice = UINT;

	struct Vertex
	{
		UINT X, Y;
		COLORREF Color = 0xFFFFFF;
	};

	struct Rect
	{
		UINT Left;
		UINT Top;
		UINT Right;
		UINT Bottom;
	};
}