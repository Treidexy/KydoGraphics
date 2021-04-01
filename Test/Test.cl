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

void DrawSeg(global uint *pixels, uint y0, uint y1, float ld, float rd, float lx, float rx)
{
	uint x0, x1;

	for (uint y = y0; y < y1; y++)
	{
		x0 = ceil(lx);
		x1 = ceil(rx);

		for (uint x = x0; x < x1; x++)
			pixels[x + y * 512] = 0x00FF00;

		lx += ld;
		rx += rd;
	}
}

#define Swap(tri, x, y) \
{ \
	tmp = tri->X[x]; tri->X[x] = tri->X[y]; tri->X[y] = tmp; \
	tmp = tri->Y[x]; tri->Y[x] = tri->Y[y]; tri->Y[y] = tmp; \
}

 #define SUB_PIX(x) (ceil((float)x) - x)

kernel void Draw(global uint *pixels, global Triangle *tris, global Rect *bounds)
{
	uint id = get_global_id(0);
	global Triangle *tri = &tris[id];

	uint tmp;
	if (tri->Y[0] > tri->Y[1])
		Swap(tri, 0, 1);
	if (tri->Y[0] > tri->Y[2])
		Swap(tri, 0, 2);
	if (tri->Y[1] > tri->Y[2])
		Swap(tri, 1, 2);

	uint i0 = tri->Y[0];
	uint i1 = tri->Y[1];
	uint i2 = tri->Y[2];

	if (i0 == i2)
		return;

	float d02 = (tri->X[2] - tri->X[0]) / (tri->Y[2] - tri->Y[0]);
	float d12 = (tri->X[2] - tri->X[1]) / (tri->Y[2] - tri->Y[1]);
	float d01 = (tri->X[1] - tri->X[0]) / (tri->Y[1] - tri->Y[0]);

	bool mid = d02 < d01;
	if (mid)
	{
		float preStep = SUB_PIX(tri->Y[0]);
		float ld = d02;

		if (i0 == i1)
		{
			float rd = d12;
			float lx = tri->X[0] + preStep * ld;
			float rx = tri->X[1] + SUB_PIX(tri->Y[1]) * rd;
			DrawSeg(pixels, i0, i2, ld, rd, lx, rd);
		}
		else if (i0 < i1)
		{
			float rd = d01;
			float lx = tri->X[0] + preStep * ld;
			float rx = tri->X[0] + preStep * rd;
			DrawSeg(pixels, i0, i1, ld, rd, lx, rd);
		}
		else if (i1 < i2)
		{
			float rd = d12;
			float rx = tri->X[1] + SUB_PIX(tri->Y[1]) * rd;
			DrawSeg(pixels, i1, i2, ld, rd, 0, rd);
		}
	}
	
	// for (uint y = bounds->Top; y <= bounds->Bottom; y++)
	// 	for (uint x = bounds->Left; x <= bounds->Right; x++)
	// 		pixels[x + y * 512] = 0x00FF00;
}