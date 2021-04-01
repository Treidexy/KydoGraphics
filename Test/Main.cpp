#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
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
			while (wnd.IsAlive() && renderer->IsAlive())
			{
				UINT x0 = std::rand() % 512, x1 = std::rand() % 512, x2 = std::rand() % 512;
				UINT y0 = std::rand() % 512, y1 = std::rand() % 512, y2 = std::rand() % 512;
				Kydo::Vertex x = { x0, y0, };
				Kydo::Vertex y = { x1, y1, };
				Kydo::Vertex z = { x2, y2, };
				Kydo::Triangle tri = { x, y, z, };
				//printf("%i, %i; %i, %i; %i, %i\n", x0, y0, x1, y1, x2, y2);
				renderer->Render(tri);
				wnd.Update();
				wnd.Clear();
				wnd.Render(renderer);

				using namespace std::chrono_literals;
				std::this_thread::sleep_for(50ms);
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}