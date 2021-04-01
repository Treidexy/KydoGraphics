#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <cstdlib>
#include <algorithm>

static UINT titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

static float Lerp(float x, float y, float t)
{ return x + t * (y - x); }

void DrawLine(COLORREF *pixels, const Kydo::Vertex &v0, const Kydo::Vertex &v1)
{
	const Kydo::Vertex &min = (v0.Y < v1.Y) ? v0 : v1;
	const Kydo::Vertex &max = (v0.Y < v1.Y) ? v1 : v0;

	INT dx = max.X - min.X;
	INT dy = max.Y - min.Y;

	if (dx == 0)
		dx = 1;
	if (dy == 0)
		dy = 1;

	INT ror = (dx * min.Y) / dy;
	INT b = min.X - ror;

	for (UINT y = min.Y; y <= max.Y; y++)
	{
		INT ror = (dx * y) / dy;
		INT x = ror + b;

		pixels[x + (512 - y - titleBarHeight - 1) * 512] = 0x00FF00;
	}
}

void DrawTriangle(COLORREF *pixels, const Kydo::Triangle &tri)
{
	Kydo::Rect bounds
	{
		std::min({ tri.Vertices[0].X, tri.Vertices[1].X, tri.Vertices[2].X }),
		std::min({ tri.Vertices[0].Y, tri.Vertices[1].Y, tri.Vertices[2].Y }),
		std::max({ tri.Vertices[0].X, tri.Vertices[1].X, tri.Vertices[2].X }),
		std::max({ tri.Vertices[0].Y, tri.Vertices[1].Y, tri.Vertices[2].Y }),
	};

	for (UINT y = bounds.Top; y < bounds.Bottom; y++)
	{
		//float y1 = Lerp(tri.Vertices[0].Y, tri.Vertices[1].Y, y - bounds.Top);
		//float y2 = Lerp(tri.Vertices[1].Y, tri.Vertices[2].Y, y - bounds.Top);
		//float y3 = Lerp(tri.Vertices[2].Y, tri.Vertices[0].Y, y - bounds.Top);
		float yr = float(y - bounds.Top) / float(bounds.Bottom - bounds.Top);

		float minX =  INFINITY;
		float maxX = -INFINITY;
		for (float rx = 0; rx < bounds.Right - bounds.Left; rx++)
		{
			float x1 = Lerp(tri.Vertices[0].X, tri.Vertices[1].X, yr);
			float x2 = Lerp(tri.Vertices[1].X, tri.Vertices[2].X, yr);
			float x3 = Lerp(tri.Vertices[2].X, tri.Vertices[0].X, yr);

			minX = std::min({ minX, x1, x2, x3 });
			maxX = std::max({ maxX, x1, x2, x3 });
		}

		for (UINT x = minX; x < maxX; x++)
			pixels[x + (511 - y + titleBarHeight) * 512] = 0x00FF00;
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
		//auto renderer = Kydo::Renderer::Create(wnd, src);
		//if (renderer->IsAlive())
		//{
			wnd.Show();
			//DrawLine(wnd.Pixels, { 256, 128 }, { 128, 384 });
			//DrawLine(wnd.Pixels, { 128, 384 }, { 384, 384 });
			//DrawLine(wnd.Pixels, { 384, 384 }, { 256, 128 });
			DrawTriangle(wnd.Pixels, Kydo::Triangle
				{
					Kydo::Vertex { 256, 128, },
					Kydo::Vertex { 128, 384, },
					Kydo::Vertex { 384, 384, },
				});
			//renderer->Render(
			//	{
			//		256, 128, 384,
			//		128, 384, 384,
			//	});
			while (wnd.IsAlive()) // && renderer->IsAlive())
			{
				wnd.Update();
				wnd.Render();
			}
			wnd.Destroy();
		//}
	}

	std::system("pause");
}