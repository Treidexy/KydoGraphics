#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <thread>
#include <cstdlib>
#include <algorithm>

using uint = UINT;

// https://github.com/tuliosouza99/rasterization
void DrawPixel(COLORREF *pixels, const Kydo::Vertex &a)
{ pixels[a.X + (a.Y + 23) * 512] = a.Color; }

void DrawLine(COLORREF *pixels, const Kydo::Vertex &a, const Kydo::Vertex &b)
{
	int dx, dy, d, incrE, incrNe, step;
	Kydo::Vertex newVert = a;
	newVert.Color = 0x0000FF;
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

void DrawTriangle(COLORREF *pixels, const Kydo::Triangle &tri)
{
	const Kydo::Vertex &a = tri.Vertices[0], &b = tri.Vertices[1], &c = tri.Vertices[2];
	DrawLine(pixels, a, b);
	DrawLine(pixels, b, c);
	DrawLine(pixels, c, a);

	uint minX = std::min({ a.X, b.X, c.X });
	uint maxX = std::max({ a.X, b.X, c.X });
	
	uint minY = std::min({ a.Y, b.Y, c.Y });
	uint maxY = std::max({ a.Y, b.Y, c.Y });
	
	Kydo::Vertex eab { b.X - a.X, b.Y - a.Y };
	Kydo::Vertex eac { c.X - a.X, c.Y - a.Y };
	
	for (uint y = minY; y <= maxY; y++)
		for (uint x = minX; x <= maxX; x++)
		{
			Kydo::Vertex q(x - a.X, y - a.Y);
	
			float s = float((q.X * eac.Y) - (eac.X * q.Y)) / float((eab.X * eac.Y) - (eac.X * eab.Y));
			float t = float((eab.X * q.Y) - (q.X * eab.Y)) / float((eab.X * eac.Y) - (eac.X * eab.Y));
	
			if (s >= 0 && t >= 0 && s + t <= 1)
				pixels[x + (y + 23) * 512] = 0x00FF00;
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
			wnd.Show();
			std::srand(std::time(NULL));
			//renderer->Render(tri);
			//wnd.Render(renderer);
			while (wnd.IsAlive() && renderer->IsAlive())
			{
				wnd.Clear();
				UINT x0 = std::rand() % 512, x1 = std::rand() % 512, x2 = std::rand() % 512;
				UINT y0 = std::rand() % 512, y1 = std::rand() % 512, y2 = std::rand() % 512;
				Kydo::Vertex x { x0, y0, };
				Kydo::Vertex y { x1, y1, };
				Kydo::Vertex z { x2, y2, };
				Kydo::Triangle tri { x, y, z, };
				DrawTriangle(wnd.Pixels, tri);
				//renderer->Render(tri);
				wnd.Render(renderer);
				wnd.Update();

				using namespace std::chrono_literals;
				std::this_thread::sleep_for(500ms);
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}