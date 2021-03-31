#include "Kydo/Window.h"

namespace Kydo
{
	Window::Window(PCWSTR title, LONG width, LONG height, PCWSTR className, HMODULE module)
		: module(module), title(title), className(className), rect { 0, 0, width, height }
	{
		if (!module)
			module = GetModuleHandleW(NULL);

		clazz = { sizeof(WNDCLASSEXW) };
		clazz.hInstance = module;
		clazz.lpszClassName = className;
		RegisterClassExW(&clazz);

		RECT screen;
		GetWindowRect(GetDesktopWindow(), &screen);
		handle = CreateWindowExW(0, className, title, WS_OVERLAPPEDWINDOW, (screen.right - rect.right) >> 1, (screen.bottom - rect.right) >> 1, rect.right, rect.bottom, NULL, NULL, module, NULL);
	}


	void Window::Show()
	{ ShowWindowAsync(handle, TRUE); }

	void Window::Hide()
	{ ShowWindowAsync(handle, FALSE); }


	void Window::Update()
	{
		if (GetMessageW(&msg, handle, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window::Destroy()
	{
		if (!destroyed)
		{
			DestroyWindow(handle);
			UnregisterClassW(className, module);

			destroyed = true;
		}
	}

	bool Window::IsAlive()
	{ return alive; }
}