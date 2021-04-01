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

struct Point
{
	uint X, Y;
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

void DrawBottomFlat(global uint *pixels, global Triangle *tri)
{
	uint y = get_global_id(0);

	float i1 = (tri->X[1] - tri->X[0]) / (tri->Y[1] - tri->Y[0]);
	float i2 = (tri->X[2] - tri->X[0]) / (tri->Y[2] - tri->Y[0]);

	float c1 = tri->X[0];
	float c2 = tri->X[0];
	
	c1 += i1 * (y - tri->Y[0]);
	c2 += i2 * (y - tri->Y[0]);

	for (uint x = c1; x <= c2; x++)
		pixels[x + y * 512] = 0x00FF00;
}

void DrawTopFlat(global uint *pixels, global Triangle *tri)
{
	float m0 = (tri->X[2] - tri->X[0]) / (tri->Y[2] - tri->Y[0]);
	float m1 = (tri->X[2] - tri->X[1]) / (tri->Y[2] - tri->Y[1]);

	uint yStart = ceil(tri->Y[0] - 0.5f);
	uint yEnd = ceil(tri->Y[2] - 0.5f);

	for (uint y = yStart; y < yEnd; y++)
	{
		float p0 = m0 * (y + 0.5f - tri->Y[0]) + tri->X[0];
		float p1 = m1 * (y + 0.5f - tri->Y[1]) + tri->X[1];

		uint xStart = ceil(p0 - 0.5f);
		uint xEnd = ceil(p1 - 0.5f);

		for (uint x = xStart; x < xEnd; x++)
			pixels[x + y * 512] = 0x00FF00;
	}
}

kernel void Draw(global uint *pixels, global Triangle *tris, global Rect *bounds)
{
	uint id = get_global_id(0);
	//DrawTopFlat(pixels, &tris[id]);
	for (uint y = bounds->Top; y <= bounds->Bottom; y++)
		for (uint x = bounds->Left; x <= bounds->Right; x++)
			pixels[x + y * 512] = 0x00FF00;
}