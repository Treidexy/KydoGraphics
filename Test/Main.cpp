#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <thread>
#include <cstdlib>
#include <algorithm>

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
			UINT x0 = std::rand() % 512, x1 = std::rand() % 512, x2 = std::rand() % 512;
			UINT y0 = std::rand() % 512, y1 = std::rand() % 512, y2 = std::rand() % 512;
			Kydo::Vertex x = { 128, 348, };
			Kydo::Vertex y = { 256, 128, };
			Kydo::Vertex z = { 384, 256, };
			Kydo::Triangle tri = { x, y, z, };
			renderer->Render(tri);
			wnd.Render(renderer);
			while (wnd.IsAlive() && renderer->IsAlive())
			{
				wnd.Update();
				//wnd.Clear();
				//wnd.Render(renderer);

				//using namespace std::chrono_literals;
				//std::this_thread::sleep_for(50ms);
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}