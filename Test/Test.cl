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
	uint y = get_global_id(0);

	float i1 = (tri->X[2] - tri->X[0]) / (tri->Y[2] - tri->Y[0]);
	float i2 = (tri->X[2] - tri->X[1]) / (tri->Y[2] - tri->Y[1]);

	float c1 = tri->X[2];
	float c2 = tri->X[2];
	
	c1 += i1 * (y - tri->Y[3]);
	c2 += i2 * (y - tri->Y[0]);

	for (uint x = c1; x <= c2; x++)
		pixels[x + y * 512] = 0x00FF00;
}

kernel void Draw(global uint *pixels, global Triangle *tri, global Rect *bounds)
{
	uint y = get_global_id(0);
	if (y < bounds->Top || y > bounds->Bottom)
		return;

	DrawBottomFlat(pixels, tri);
	DrawTopFlat(pixels, tri);
}