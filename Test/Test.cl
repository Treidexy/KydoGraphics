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

kernel void Draw(global uint *pixels, global Triangle *tri, global Rect *bounds)
{
	uint y = get_global_id(0);
	if (y < bounds->Top || y > bounds->Bottom)
		return;

	bounds->Left = min(tri->X[0], min(tri->X[1], tri->X[2]));
	bounds->Right = max(tri->X[0], max(tri->X[1], tri->X[2]));
	for (uint x = bounds->Left; x <= bounds->Right; x++)
	{
		pixels[x + y * 512] = 0x00FF00;
	}
}