#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <thread>
#include <cstdlib>
#include <algorithm>

#define PI 3.1415926535f
#define TWO_PI PI * 2

using uint = UINT;
using namespace Kydo;

#define SAFE 1
#define Swap(x, y) { tmp = x; x = y; y = tmp; }

void DrawPixel(Color *pixels, uint x, uint y, Color col)
{
	#if SAFE
	if (x < 512 && y < 512)
	#endif
		pixels[x + y * 512] = col;
}

void DrawBottom(Color *pixels, Vertex a, Vertex b, Vertex c)
{
	// Calculate inverted slope
	float is1 = (float)(int)(b.X - a.X) / (float)(int)(b.Y - a.Y);
	float is2 = (float)(int)(c.X - a.X) / (float)(int)(c.Y - a.Y);

	// Calculate current x
	float cx1 = a.X;
	float cx2 = a.X;

	for (uint y = a.Y; y <= b.Y; y++)
	{
		for (int x = std::min(cx1, cx2), end = std::max(cx1, cx2); x <= end; x++)
			DrawPixel(pixels, x, y, 0xFFFFFF);
		cx1 += is1;
		cx2 += is2;
	}
}

void DrawTop(Color *pixels, Vertex a, Vertex b, Vertex c)
{
	// Calculate inverted slope
	float is1 = (float)(int)(c.X - a.X) / (float)(int)(c.Y - a.Y);
	float is2 = (float)(int)(c.X - b.X) / (float)(int)(c.Y - b.Y);

	// Calculate current
	float cx1 = c.X;
	float cx2 = c.X;

	for (uint y = c.Y; y > a.Y; y--)
	{
		for (int x = std::min(cx1, cx2), end = std::max(cx1, cx2); x <= end; x++)
			DrawPixel(pixels, x, y, 0xFFFFFF);
		cx1 -= is1;
		cx2 -= is2;
	}
}

void Draw(uint id, Color *pixels, std::vector<Vertex> verts, std::vector<Indice> indices)
{
	Vertex
		*a = &verts[indices[id * 3 + 0]],
		*b = &verts[indices[id * 3 + 1]],
		*c = &verts[indices[id * 3 + 2]];

	Vertex *tmp;
	if (a->Y > b->Y)
		Swap(a, b);
	if (a->Y > c->Y)
		Swap(a, c);
	if (b->Y > c->Y)
		Swap(b, c);

	if (b->Y == c->Y)
		DrawBottom(pixels, *a, *b, *c);
	else if (a->Y == b->Y)
		DrawTop(pixels, *a, *b, *c);
	else
	{
		// Cut the triangle in half (totally not torture)
		Vertex d = {
			a->X + ((float)(int)(b->Y - a->Y) / (float)(int)(c->Y - a->Y)) * (int)(c->X - a->X),
			b->Y,
		};
		DrawBottom(pixels, *a, *b, d);
		DrawTop(pixels, *b, d, *c);
	}
}

int main()
{
	{
		std::string src;
		{
			FILE *fs = fopen("Test.cl", "rb");
			fseek(fs, 0, SEEK_END);
			long len = ftell(fs);
			src.resize(len);
			rewind(fs);
			fread(src.data(), 1, len, fs);
			fclose(fs);
		}

		Kydo::Window wnd(L"Kydo Test", 512, 512);
		auto renderer = Kydo::Renderer::Create(wnd, src);
		if (renderer->IsAlive())
		{
			float rad = 128;
			float rot = 0;
			std::vector<Kydo::Vertex> verts = {
				{ 128, 384, 0xFF0000, },
				{ 384, 384, 0x00FF00, },
				{ 384, 128, 0x0000FF, },
				{ 128, 128, 0xFFFF00, },
			};
			std::vector<Kydo::Indice> indices = {
				0, 1, 2,
				0, 2, 3,
			};

			int nFrames = 0;

			wnd.Show();
			while (wnd.IsAlive() && renderer->IsAlive())
			{
				rot += PI / 180;
				rot = std::fmodf(rot, PI);
				for (size_t i = 0; i < verts.size(); i++)
				{
					verts[i].X = sinf(rot + (i * TWO_PI / verts.size())) * rad + 256;
					verts[i].Y = cosf(rot + (i * TWO_PI / verts.size())) * rad + 256;
				}

				wnd.Update();
				wnd.Clear(renderer);
				renderer->Render(verts, indices);
				//Draw(0, wnd.Pixels, verts, indices);
				//Draw(1, wnd.Pixels, verts, indices);
				wnd.Render(renderer);
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(50ms);
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}