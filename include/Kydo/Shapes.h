#pragma once

#include "Kydo/Core.h"

namespace Kydo
{
	struct Triangle
	{
		UINT X[3];
		UINT Y[3];
	};

	struct Rect
	{
		UINT Left;
		UINT Top;
		UINT Right;
		UINT Bottom;
	};
}