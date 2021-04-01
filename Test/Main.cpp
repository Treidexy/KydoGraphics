#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <cstdlib>
#include <algorithm>

static UINT titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

int FindCommonDenominator(int x, int y)
{
	if (x == 0 || y == 0)
		return 1;

	x = x < 0 ? -x : x;
	y = y < 0 ? -y : y;

	while (x != y)
		if (x > y)
			x -= y;
		else
			y -= x;
	return x;
}

void DrawLine(COLORREF *pixels, const Kydo::Vertex &v0, const Kydo::Vertex &v1)
{
	const Kydo::Vertex &min = (v0.Y < v1.Y) ? v0 : v1;
	const Kydo::Vertex &max = (v0.Y < v1.Y) ? v1 : v0;

	INT dx = min.X < max.X ? max.X - min.X : min.X - max.X;
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
		INT minX = ror + b;
		INT iror = (dx * (minX)) / dy;
		INT maxX = iror + b;


		std::printf("%i, %i\n", minX, maxX);
		//for (UINT x = minX; x <= maxX; x++)
		//	pixels[x + (512 - y - titleBarHeight - 1) * 512] = 0x00FF00;
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
			DrawLine(wnd.Pixels, { 128, 384 }, { 384, 384 });
			//DrawLine(wnd.Pixels, { 384, 384 }, { 256, 128 });
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