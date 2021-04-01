#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <cstdlib>
#include <algorithm>

static UINT titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

using uint = UINT;

static float Lerp(float x, float y, float t)
{ return x + t * (y - x); }

static float Min(float x, float y)
{ return x < y ? x : y; }

static float Max(float x, float y)
{ return x > y ? x : y; }

void DrawTriangle(COLORREF *pixels, const Kydo::Triangle &tri)
{
	Kydo::Vertex verts[3] =
	{
		tri.Vertices[0],
		tri.Vertices[1],
		tri.Vertices[2],
	};

	// Sort the vertices from smallest to greatest (On Y-Axis)
	Kydo::Vertex tmp;
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
			//DrawTriangle(wnd.Pixels, Kydo::Triangle
			//	{
			//		Kydo::Vertex { 256, 128, },
			//		Kydo::Vertex { 128, 384, },
			//		Kydo::Vertex { 384, 384, },
			//	});
			renderer->Render(Kydo::Triangle
				{
					Kydo::Vertex { 256, 128, },
					Kydo::Vertex { 128, 384, },
					Kydo::Vertex { 384, 384, },
				});
			while (wnd.IsAlive() && renderer->IsAlive())
			{
				wnd.Update();
				wnd.Render(renderer);
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}