#define _CRT_SECURE_NO_WARNINGS
#include <Kydo.h>
#include <cstdlib>

int main()
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

	Kydo::Window wnd(L"Kydo Test");
	auto renderer = Kydo::Renderer::Create(wnd, src);
	if (renderer->IsAlive())
	{
		wnd.Show();
		while (wnd.IsAlive())
		{
			renderer->Render();

			wnd.Update();
			wnd.Render(renderer);
		}
		wnd.Destroy();
	}

	std::system("pause");
}