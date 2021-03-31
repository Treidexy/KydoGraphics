#include "Kydo/Window.h"

namespace Kydo
{
	Window::Window(PCWSTR title, LONG width, LONG height, PCWSTR className, HMODULE mod)
		: module(mod), title(title), className(className), rect { 0, 0, width, height }
	{
		if (!module)
			module = GetModuleHandleW(NULL);

		clazz = { sizeof(WNDCLASSEXW) };
		clazz.hInstance = module;
		clazz.lpszClassName = className;
		clazz.lpfnWndProc = DefWindowProcW;
		RegisterClassExW(&clazz);

		RECT screen;
		GetWindowRect(GetDesktopWindow(), &screen);
		rect.left = (screen.right - rect.right) >> 1;
		rect.top = (screen.bottom - rect.bottom) >> 1;
		handle = CreateWindowExW(0, className, title, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, module, NULL);
	}

	Window::~Window()
	{ Destroy(); }


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