#pragma once

#define NOMINMAX
#include <Windows.h>
#include <memory>
#include "Kydo/Core.h"

namespace Kydo
{
	class KYDO_API Renderer;

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

		LONG nPixels;
		LONG x, y, width, height;
		COLORREF *pixels;

		Renderer *renderer;
		bool alive = true, destroyed = false;
	public:
		Window(PCWSTR title, LONG width = 1280, LONG height = 720, PCWSTR className = L"MyClass", HMODULE = NULL);
		~Window();

		void Show();
		void Hide();

		void Update();
		void Clear();
		void Render();
		void Render(const std::unique_ptr<Renderer> &);
		void Destroy();

		bool IsAlive();
		LONG GetX();
		LONG GetY();
		LONG GetWidth();
		LONG GetHeight();

		COLORREF *GetPixels();
		LONG GetPixelCount();

		KYDO_PROPERTY(get = GetX) LONG X;
		KYDO_PROPERTY(get = GetY) LONG Y;
		KYDO_PROPERTY(get = GetWidth) LONG Width;
		KYDO_PROPERTY(get = GetHeight) LONG Height;

		KYDO_PROPERTY(get = GetPixels) COLORREF *Pixels;
		KYDO_PROPERTY(get = GetPixelCount) LONG PixelCount;

		static LRESULT CALLBACK KydoWinProc(__in HWND wnd, __in UINT msg, __in WPARAM wParam, __in LPARAM lParam);
	};
}

#include "Kydo/Renderer.h"