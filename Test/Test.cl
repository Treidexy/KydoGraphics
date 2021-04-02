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

static float Dist(global Vertex *a, global Vertex *b)
{
	float dx = Abs((float)a->X - (float)b->X);
	float dy = Abs((float)a->Y - (float)b->Y);
	
	return sqrt(dx * dx + dy * dy);
}

static void DrawPixel(global uint *pixels, uint x, uint y, uint col)
{ pixels[x + y * 512] = col; }

static void DrawVertex(global uint *pixels, Vertex *vert)
{ DrawPixel(pixels, vert->X, vert->Y, vert->Color); }

// Taken from -> https://github.com/tuliosouza99/rasterization
void DrawLine(global uint *pixels, global Vertex *a, global Vertex *b)
{
	int dx, dy, d, incrE, incrNe, step;
	Vertex newVert = *a;
	dx = Abs((int)(b->X - a->X));
	dy = Abs((int)(b->Y - a->Y));
	
	float finalStep = sqrt((float)(dx * dx + dy * dy));
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
	
	float d1 = Dist(a, b);
	float d2 = Dist(b, c);
	float d3 = Dist(c, a);
	// float o = Abs(((float)b->X - (float)a->X) * ((float)c->Y - (float)a->Y) - ((float)c->X -(float)a->X) * ((float)b->Y - (float)a->Y));
	for (uint y = minY; y <= maxY; y++)
		for (uint x = minX; x <= maxX; x++)
		{
			// float a1 = Abs(((float)a->X - (float)x) * ((float)b->Y - (float)y) - ((float)b->X - (float)x) * ((float)a->Y - (float)y));
			// float a2 = Abs(((float)b->X - (float)x) * ((float)c->Y - (float)y) - ((float)c->X - (float)x) * ((float)b->Y - (float)y));
			// float a3 = Abs(((float)c->X - (float)x) * ((float)a->Y - (float)y) - ((float)a->X - (float)x) * ((float)c->Y - (float)y));

			// if (a1 + a2 + a3 == o)
				// DrawPixel(pixels, x, y, 0xFFFFFF);
			
			Vertex *pv = &(Vertex) { x, y };
			float pd1 = Dist(pv, a);
			float pd2 = Dist(pv, b);
			float pd3 = Dist(pv, c);
			
			if (pd1 <= d1 && pd2 <= d2 && pd3 <= d3)
				DrawPixel(pixels, x, y, 0xFFFFFF);
		}
	
	DrawLine(pixels, a, b);
	DrawLine(pixels, b, c);
	DrawLine(pixels, c, a);
}

kernel void Clear(global uint *pixels, uint col)
{
	uint y = get_global_id(0);
	for (uint x = 0; x < 512; x++)
		pixels[x + y * 512] = col;
}