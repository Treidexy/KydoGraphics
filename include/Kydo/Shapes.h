#pragma once

#include "Kydo/Core.h"

namespace Kydo
{
	struct Vertex
	{
		UINT X, Y;
		COLORREF Color;
	};

	struct Triangle
	{
		Vertex Vertices[3];
	};

	struct Rect
	{
		UINT Left;
		UINT Top;
		UINT Right;
		UINT Bottom;
	};
}