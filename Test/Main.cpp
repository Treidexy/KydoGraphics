#include <Kydo.h>

int main()
{
	Kydo::Window wnd(L"Kydo Test");
	wnd.Show();

	while (wnd.IsAlive())
		wnd.Update();
	wnd.Destroy();

	system("pause");
}