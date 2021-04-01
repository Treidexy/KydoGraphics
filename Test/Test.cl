typedef struct Triangle Triangle;
typedef struct Rect Rect;

struct Triangle
{
	float X[3];
	float Y[3];
};

struct Rect
{
	float Left;
	float Top;
	float Right;
	float Bottom;
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

kernel void Draw(global uint *pixels, global Triangle *tris, global Rect *bounds)
{
	uint y = get_global_id(0);

	for (uint x = bounds.Left; x <= bounds.Right; x++)
		if (y >= bounds.Top && y <= bounds.Bottom)
			pixels[id] = 0x00FF00;
}