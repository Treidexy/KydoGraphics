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
			std::srand(std::time(NULL));
			wnd.Show();
			while (wnd.IsAlive() && renderer->IsAlive())
			{
				UINT r0 = std::rand() % 0xFF, r1 = std::rand() % 0xFF, r2 = std::rand() % 0xFF;
				UINT g0 = std::rand() % 0xFF, g1 = std::rand() % 0xFF, g2 = std::rand() % 0xFF;
				UINT b0 = std::rand() % 0xFF, b1 = std::rand() % 0xFF, b2 = std::rand() % 0xFF;

				//UINT r0 = 0xFF, r1 = 0x00, r2 = 0x00;
				//UINT g0 = 0x00, g1 = 0xFF, g2 = 0x00;
				//UINT b0 = 0x00, b1 = 0x00, b2 = 0xFF;

				Kydo::Vertex x { 256, 384, r0 << 16 | g0 << 8 | b0 << 0, };
				Kydo::Vertex y { 128, 128, r1 << 16 | g1 << 8 | b1 << 0, };
				Kydo::Vertex z { 384, 128, r2 << 16 | g2 << 8 | b2 << 0, };
				Kydo::Triangle tri { x, y, z, };

				wnd.Update();
				wnd.Clear(renderer);
				renderer->Render(tri);
				wnd.Render(renderer);
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(50ms);
			}
			wnd.Destroy();
		}
	}

	std::system("pause");
}