#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <cstdlib>

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
			renderer->Render(
				{
					256, 128, 384,
					128, 384, 384,
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