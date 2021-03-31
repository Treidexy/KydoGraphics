#pragma once

#define NOMINMAX
#include <Windows.h>
#include "Kydo/Core.h"

namespace Kydo
{
	class KYDO_API Window
	{
	private:
		static Window *Instance;

		HDC dc, bmpDc;
		MSG msg {};
		HWND handle;
		HMODULE module;

		PCWSTR className, title;
		WNDCLASSEXW clazz;

		LONG x, y, width, height;
		COLORREF *pixels;

		bool alive = true, destroyed = false;
	public:
		Window(PCWSTR title, LONG width = 1280, LONG height = 720, PCWSTR className = L"MyClass", HMODULE = NULL);
		~Window();

		void Show();
		void Hide();

		void Update();
		void Destroy();

		bool IsAlive();

		static LRESULT CALLBACK KydoWinProc(__in HWND wnd, __in UINT msg, __in WPARAM wParam, __in LPARAM lParam);
	};
}