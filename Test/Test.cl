typedef struct Triangle Triangle;
typedef struct Rect Rect;
typedef struct Point Point;

struct Triangle
{
	uint X[3];
	uint Y[3];
};

struct Rect
{
	uint Left;
	uint Top;
	uint Right;
	uint Bottom;
};

uint Hash(uint n)
{
	n ^= 2747636419;
	n *= 2654435769;

	n ^= n >> 16;
	n *= 2654435769;

	n ^= n >> 16;
	n *= 2654435769;
	return n;
}

void DrawLine(global uint *pixels, uint x0, uint y0, uint x1, uint y1)
{
	uint y = get_global_id(0);

	uint dx = abs(x0 - x1);
	uint dy = abs(y0 - y1);
	uint d = 2 * dy - dx;
	uint cy = min(y0, y1);

	for (uint x = x0; x <= x1; x++)
	{
		if (cy == y)
			pixels[x + y * 512] = 0x00FF00;
		if (d > 0)
		{
			cy++;
			d = d - 2 * dx;
		}
		d += 2 * dy;
	}
}

kernel void Draw(global uint *pixels, global Triangle *tri, global Rect *bounds)
{
	uint y = get_global_id(0);
	if (y < bounds->Top || y > bounds->Bottom)
		return;

	DrawLine(pixels, tri->X[0], tri->Y[0], tri->X[1], tri->Y[1]);
	DrawLine(pixels, tri->X[1], tri->Y[1], tri->X[2], tri->Y[2]);
	DrawLine(pixels, tri->X[2], tri->Y[2], tri->X[0], tri->Y[0]);
	// for (uint x = bounds->Left; x <= bounds->Right; x++)
	// {
	// 	pixels[x + y * 512] = 0x00FF00;
	// }
}