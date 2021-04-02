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

// Taken from -> https://github.com/tuliosouza99/rasterization
kernel void Draw(global uint *pixels, global Triangle *tris, uint titleBarHeight)
{
	uint id = get_global_id(0);
	global Vertex *a = &tris[id].Vertices[0], *b = &tris[id].Vertices[1], *c = &tris[id].Vertices[2];
	
	uint minX = Min(a->X, Min(b->X, c->X));
	uint maxX = Max(a->X, Max(b->X, c->X));
	
	uint minY = Min(a->Y, Min(b->Y, c->Y));
	uint maxY = Max(a->Y, Max(b->Y, c->Y));
	
	// Edge for a/b & a/c
	Vertex *eab = &(Vertex) { b->X - a->X, b->Y - a->Y };
	Vertex *eac = &(Vertex) { c->X - a->X, c->Y - a->Y };
	
	for (uint y = minY; y < maxY; y++)
		for (uint x = minX; x < maxX; x++)
		{
			Vertex *q = &(Vertex) { x - a->X, y - a->Y };
			
			// Math
			float s = (float)((q->X * eac->Y) - (eac->X * q->Y)) / (float)((eab->X * eac->Y) - (eac->X * eab->Y));
			float t = (float)((eab->X * q->Y) - (q->X * eab->Y)) / (float)((eab->X * eac->Y) - (eac->X * eab->Y));
			
			if (s >= 0 && t >= 0 && s + t <= 1)
				pixels[x + (y + titleBarHeight) * 512] = 0x00FF00;
		}
}