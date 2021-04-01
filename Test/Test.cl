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

static float Lerp(float x, float y, float t)
{ return x + t * (y - x); }

static float Min(float x, float y)
{ return x < y ? x : y; }

static float Max(float x, float y)
{ return x > y ? x : y; }

kernel void Draw(global uint *pixels, global Triangle *tris)
{
	uint id = get_global_id(0);
	Vertex verts[3] =
	{
		tris[id].Vertices[0],
		tris[id].Vertices[1],
		tris[id].Vertices[2],
	};
	
	// Sort the vertices from smallest to greatest (On Y-Axis)
	Vertex tmp;
	for (uint i = 0; i < 3; i++)
		for (uint j = 0; j < 3; j++)
			if (verts[i].Y < verts[j].Y)
			{
				// Do a little switch-a-roo
				tmp = verts[i];
				verts[i] = verts[j];
				verts[j] = tmp;
			}
			
	float dy = verts[2].Y - verts[0].Y;
	for (uint y = 0; y < dy; y++)
	{
		uint x0 = Lerp(verts[1].X, verts[0].X, (float)y / dy);
		uint x1 = Lerp(verts[2].X, verts[0].X, (float)y / dy);
		
		uint min = Min(x0, x1);
		uint max = Max(x0, x1);
		for (uint x = min; x < max; x++)
			pixels[x + (y + verts[0].Y + 23) * 512] = 0x00FF00;
	}
}