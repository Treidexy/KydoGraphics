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