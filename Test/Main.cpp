#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <thread>
#include <cstdlib>
#include <algorithm>

using uint = UINT;

void DrawPixel(COLORREF *pixels, uint x, uint y, COLORREF col = 0x00FF00)
{ pixels[x + (y) * 512] = col; }

void DrawPixel(COLORREF *pixels, const Kydo::Vertex &a)
{ DrawPixel(pixels, a.X, a.Y, a.Color); }

// https://github.com/tuliosouza99/rasterization
void DrawLine(COLORREF *pixels, const Kydo::Vertex &a, const Kydo::Vertex &b)
{
	int dx, dy, d, incrE, incrNe, step;
	Kydo::Vertex newVert = a;
	newVert.Color = 0xFF0000;
	dx = std::abs(int(b.X - a.X));
	dy = std::abs(int(b.Y - a.Y));
	
	float finalStep = sqrtf(dx * dx + dy * dy);
	step = 0;

	DrawPixel(pixels, newVert);
	if (dx >= dy)
	{
		incrE = 2 * dy;
		incrNe = 2 * (dy - dx);

		d = 2 * dy - dx;
		while (newVert.X != b.X)
		{
			if (d <= 0)
				d += incrE;
			else
			{
				d += incrNe;
				if (b.Y >= a.Y)
					newVert.Y++;
				else
					newVert.Y--;
			}

			if (b.X >= a.X)
				newVert.X++;
			else
				newVert.X--;

			DrawPixel(pixels, newVert);
			step++;
		}
	}
	else
	{
		incrE = 2 * dx;
		incrNe = 2 * (dx - dy);

		d = 2 * (dx - dy);
		while (newVert.Y != b.Y)
		{
			if (d <= 0)
				d += incrE;
			else
			{
				d += incrNe;
				if (b.X >= a.X)
					newVert.X++;
				else
					newVert.X--;
			}

			if (b.Y >= a.Y)
				newVert.Y++;
			else
				newVert.Y--;

			DrawPixel(pixels, newVert);
			step++;
		}
	}
}

// http://www.jeffreythompson.org/collision-detection/tri-point.php
void DrawTriangle(COLORREF *pixels, const Kydo::Triangle &tri)
{
	const Kydo::Vertex &a = tri.Vertices[0], &b = tri.Vertices[1], &c = tri.Vertices[2];

	uint minX = std::min({ a.X, b.X, c.X });
	uint maxX = std::max({ a.X, b.X, c.X });
	
	uint minY = std::min({ a.Y, b.Y, c.Y });
	uint maxY = std::max({ a.Y, b.Y, c.Y });

	float o = std::abs(((float)b.X - (float)a.X) * ((float)c.Y - (float)a.Y) - ((float)c.X - (float)a.X) * ((float)b.Y - (float)a.Y));
	for (uint y = minY; y <= maxY; y++)
		for (uint x = minX; x <= maxX; x++)
		{
			float a1 = std::abs(((float)a.X - (float)x) * ((float)b.Y - (float)y) - ((float)b.X - (float)x) * ((float)a.Y - (float)y));
			float a2 = std::abs(((float)b.X - (float)x) * ((float)c.Y - (float)y) - ((float)c.X - (float)x) * ((float)b.Y - (float)y));
			float a3 = std::abs(((float)c.X - (float)x) * ((float)a.Y - (float)y) - ((float)a.X - (float)x) * ((float)c.Y - (float)y));

			if (a1 + a2 + a3 == o)
				DrawPixel(pixels, x, y);
		}

	DrawLine(pixels, a, b);
	DrawLine(pixels, b, c);
	DrawLine(pixels, c, a);
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
			wnd.Show();
			std::srand(std::time(NULL));
			//renderer->Render(tri);
			//wnd.Render(renderer);
			while (wnd.IsAlive() && renderer->IsAlive())
			{
				UINT x0 = std::rand() % 512, x1 = std::rand() % 512, x2 = std::rand() % 512;
				UINT y0 = std::rand() % 512, y1 = std::rand() % 512, y2 = std::rand() % 512;
				UINT r0 = std::rand() % 0xFF, r1 = std::rand() % 0xFF, r2 = std::rand() % 0xFF;
				UINT g0 = std::rand() % 0xFF, g1 = std::rand() % 0xFF, g2 = std::rand() % 0xFF;
				UINT b0 = std::rand() % 0xFF, b1 = std::rand() % 0xFF, b2 = std::rand() % 0xFF;
				Kydo::Vertex x { x0, y0, r0 << 16 | g0 << 8 | b0 << 0, };
				Kydo::Vertex y { x1, y1, r1 << 16 | g1 << 8 | b1 << 0, };
				Kydo::Vertex z { x2, y2, r2 << 16 | g2 << 8 | b2 << 0, };
				Kydo::Triangle tri { x, y, z, };
				//DrawTriangle(wnd.Pixels, tri);
				wnd.Update();
				renderer->Render(tri);
				wnd.Clear(renderer);
				wnd.Render(renderer);

				using namespace std::chrono_literals;
				std::this_thread::sleep_for(50ms);
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}