#include <Kydo.h>

int main()
{
	Kydo::Window wnd(L"Kydo Test");
	wnd.Show();

	Kydo::Context context(wnd);

	while (wnd.IsAlive())
		wnd.Update();
	wnd.Destroy();

	system("pause");
}