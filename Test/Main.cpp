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
	Kydo::Vertex verts[] =
	{
		tri.Vertices[0],
		tri.Vertices[1],
		tri.Vertices[2],
	};

	Kydo::Vertex tmp;
	for (UINT i = 0; i < 3; i++)
		for (UINT j = 0; j < 3; j++)
			if (verts[i].Y > verts[j].Y)
			{
				// Do a little switch-a-roo
				tmp = verts[i];
				verts[i] = verts[j];
				verts[j] = tmp;
			}

	UINT dy = verts[0].Y - verts[2].Y;
	for (UINT y = 0; y < dy; y++)
	{
		UINT x1 = Lerp(verts[1].X, verts[2].X, float(y) / dy);
		UINT x2 = Lerp(verts[0].X, verts[2].X, float(y) / dy);

		int min = std::min(x1, x2);
		int max = std::max(x1, x2);
		for (UINT x = min; x < max; x++)
			pixels[x + (y + verts[2].Y + titleBarHeight) * 512] = 0x00FF00;
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