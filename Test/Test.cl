typedef struct Point Point;

struct Point
{
	float X, Y;
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

kernel void Draw(global uint *pixels)
{
	size_t id = get_global_id(0);
	pixels[id] = 0xFF;
}