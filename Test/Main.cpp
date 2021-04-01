#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <cstdlib>
#include <algorithm>

static UINT titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

static float Lerp(float x, float y, float t)
{ return x + t * (y - x); }

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
				wnd.Render();
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}