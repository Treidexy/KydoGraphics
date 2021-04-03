typedef struct Triangle Triangle;
typedef struct Vertex Vertex;
typedef struct Rect Rect;

struct Vertex
{
	uint X, Y;
	uint Color;
};

struct Triangle
{
	Vertex Vertices[3];
};

struct Rect
{
	uint Left;
	uint Top;
	uint Right;
	uint Bottom;
};

static uint Min(uint x, uint y)
{ return x < y ? x : y; }

static uint Max(uint x, uint y)
{ return x > y ? x : y; }

static float Abs(float x)
{
	*(int *)&x &= 0x7FFFFFFF;
	return x;
}

static uchar Lerp(float x, float y, float t)
{ return x + t * (y - x); }

static uint Blend(uint x, uint y, float t)
{
	uint out = 0;
	uint t0, t1;
	
	t0 = x >> 16 & 0xFF;
	t1 = y >> 16 & 0xFF;
	out |= Lerp(t0, t1, t) << 16;
	
	t0 = x >> 8 & 0xFF;
	t1 = y >> 8 & 0xFF;
	out |= Lerp(t0, t1, t) << 8;
	
	t0 = x >> 0 & 0xFF;
	t1 = y >> 0 & 0xFF;
	out |= Lerp(t0, t1, t) << 0;
	
	return out;
}

static float Dist(float x1, float y1, float x2, float y2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	return sqrt(Abs(dx * dx + dy * dy));
}

#define SAFE 1

static void DrawPixel(global uint *pixels, uint x, uint y, uint col)
{
	#if SAFE
	if (x < 512 && y < 512)
	#endif
		pixels[x + y * 512] = col;
}

static void DrawVertex(global uint *pixels, Vertex *vert)
{ DrawPixel(pixels, vert->X, vert->Y, vert->Color); }

// Taken from -> https://github.com/tuliosouza99/rasterization
void DrawLine(global uint *pixels, global Vertex *a, global Vertex *b)
{
	int dx, dy, d, incrE, incrNe, step;
	Vertex newVert = *a;
	dx = Abs((int)(b->X - a->X));
	dy = Abs((int)(b->Y - a->Y));
	
	float finalStep = Dist(a->X, a->Y, b->X, b->Y);
	step = 0;

	DrawVertex(pixels, &newVert);
	if (dx >= dy)
	{
		incrE = 2 * dy;
		incrNe = 2 * (dy - dx);

		d = 2 * dy - dx;
		while (newVert.X != b->X)
		{
			if (d <= 0)
				d += incrE;
			else
			{
				d += incrNe;
				if (b->Y >= a->Y)
					newVert.Y++;
				else
					newVert.Y--;
			}

			if (b->X >= a->X)
				newVert.X++;
			else
				newVert.X--;

			newVert.Color = Blend(a->Color, b->Color, (float)step / (float)finalStep);
			DrawVertex(pixels, &newVert);
			step++;
		}
	}
	else
	{
		incrE = 2 * dx;
		incrNe = 2 * (dx - dy);

		d = 2 * (dx - dy);
		while (newVert.Y != b->Y)
		{
			if (d <= 0)
				d += incrE;
			else
			{
				d += incrNe;
				if (b->X >= a->X)
					newVert.X++;
				else
					newVert.X--;
			}

			if (b->Y >= a->Y)
				newVert.Y++;
			else
				newVert.Y--;

			newVert.Color = Blend(a->Color, b->Color, (float)step / (float)finalStep);
			DrawVertex(pixels, &newVert);
			step++;
		}
	}
}

// Taken from -> http://www.jeffreythompson.org/collision-detection/tri-point.php
kernel void Draw(global uint *pixels, global Triangle *tris, uint titleBarHeight)
{
	uint id = get_global_id(0);
	global Vertex *a = &tris[id].Vertices[0], *b = &tris[id].Vertices[1], *c = &tris[id].Vertices[2];
	
	uint minX = Min(a->X, Min(b->X, c->X));
	uint maxX = Max(a->X, Max(b->X, c->X));
	
	uint minY = Min(a->Y, Min(b->Y, c->Y));
	uint maxY = Max(a->Y, Max(b->Y, c->Y));
	
	float tdab = Dist(a->X, a->Y, b->X, b->Y);
	float tdbc = Dist(b->X, b->Y, c->X, c->Y);
	float tdca = Dist(c->X, c->Y, a->X, a->Y);
	
	float o = Abs(((float)b->X - (float)a->X) * ((float)c->Y - (float)a->Y) - ((float)c->X -(float)a->X) * ((float)b->Y - (float)a->Y));
	for (uint y = minY; y <= maxY; y++)
		for (uint x = minX; x <= maxX; x++)
		{
			float areaA = Abs(((float)a->X - (float)x) * ((float)b->Y - (float)y) - ((float)b->X - (float)x) * ((float)a->Y - (float)y));
			float areaB = Abs(((float)b->X - (float)x) * ((float)c->Y - (float)y) - ((float)c->X - (float)x) * ((float)b->Y - (float)y));
			float areaC = Abs(((float)c->X - (float)x) * ((float)a->Y - (float)y) - ((float)a->X - (float)x) * ((float)c->Y - (float)y));

			if (areaA + areaB + areaC == o)
			{
				float max = Max(areaA, Max(areaB, areaC));
				uint col;
				if (max == areaA)
					col = c->Color;
				else if (max == areaB)
					col = a->Color;
				else if (max == areaC)
					col = b->Color;
				DrawPixel(pixels, x, y, col);
			}
		}
	
	DrawLine(pixels, a, b);
	DrawLine(pixels, b, c);
	DrawLine(pixels, c, a);
}

kernel void Clear(global uint *pixels, uint col)
{
	uint y = get_global_id(0);
	for (uint x = 0; x < 512; x++)
		DrawPixel(pixels, x, y, 0x404040);
}