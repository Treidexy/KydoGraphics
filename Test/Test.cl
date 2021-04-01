typedef struct Triangle Triangle;

struct Triangle
{
	float X[3];
	float Y[3];
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

kernel void Draw(global uint *pixels, global Triangle *tris, global uint *nTris)
{
	uint id = get_global_id(0);
	uint x = id % 512, y = id / 512;
	for (uint i = 0; i < *nTris; i++)
	{
		float minX = min(min(tris[i].X[0], tris[i].X[1]), tris[i].X[2]);
		float maxX = max(max(tris[i].X[0], tris[i].X[1]), tris[i].X[2]);
		float minY = min(min(tris[i].Y[0], tris[i].Y[1]), tris[i].Y[2]);
		float maxY = max(max(tris[i].Y[0], tris[i].Y[1]), tris[i].Y[2]);

		if (x >= minX && x <= minX)
			if (y >= minY && y <= maxY)
				pixels[id] = 0x00FF00;
	}
}