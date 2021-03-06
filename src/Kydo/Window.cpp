#include "Kydo/Window.h"

#include "Kydo/CLRenderer.h"

namespace Kydo
{
	Window *Window::Instance;

	LRESULT CALLBACK Window::KydoWinProc(__in HWND wnd, __in UINT msg, __in WPARAM wParam, __in LPARAM lParam)
	{
		if (!Instance->alive)
		{
			PostQuitMessage(0);
			return 0;
		}

		switch (msg)
		{
		case WM_CLOSE:
		case WM_DESTROY:
			Instance->alive = false;
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			if (Instance->pixels)
			{
				PAINTSTRUCT paint;
				HDC dc = BeginPaint(wnd, &paint);
				BitBlt(dc, 0, 0, Instance->width, Instance->height, Instance->bmpDc, 0, 0, SRCCOPY);
				EndPaint(wnd, &paint);
			}
			return 0;

		default:
			return DefWindowProcW(wnd, msg, wParam, lParam);
		}
	}

	Window::Window(PCWSTR title, LONG w, LONG h, PCWSTR className, HMODULE mod)
		: module(mod), title(title), className(className), width(w), height(h), nPixels(w * h)
	{
		Instance = this;

		if (!module)
			module = GetModuleHandleW(NULL);

		clazz = { sizeof(WNDCLASSEXW) };
		clazz.hInstance = module;
		clazz.lpszClassName = className;
		clazz.lpfnWndProc = KydoWinProc;
		RegisterClassExW(&clazz);

		RECT screen;
		GetWindowRect(GetDesktopWindow(), &screen);
		x = (screen.right - width) >> 1;
		y = (screen.bottom - height) >> 1;
		handle = CreateWindowExW(0, className, title, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
			x, y, width, height, NULL, NULL, module, NULL);

		bmpInfo = { sizeof(BITMAPINFO) };
		bmpInfo.bmiHeader.biWidth = width;
		bmpInfo.bmiHeader.biHeight = height;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = sizeof(COLORREF) * 8;
		bmpInfo.bmiHeader.biCompression = BI_RGB;

		dc = GetDC(handle);
		bmp = CreateDIBSection(dc, &bmpInfo, DIB_RGB_COLORS, (void **)&pixels, NULL, 0); assert(bmp);
		bmpDc = CreateCompatibleDC(dc); assert(bmpDc);
		SelectObject(bmpDc, bmp);
	}

	Window::~Window()
	{ Destroy(); }


	void Window::Show()
	{
		if (!alive)
			return;
		ShowWindowAsync(handle, TRUE);
	}

	void Window::Hide()
	{
		if (!alive)
			return;
		ShowWindowAsync(handle, FALSE);
	}


	void Window::Update()
	{
		if (!alive)
			return;

		if (PeekMessageW(&msg, handle, 0, 0, PM_REMOVE | PM_NOYIELD))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window::Clear(const std::unique_ptr<Renderer> &renderer)
	{
		if (!alive)
			return;
		CLRenderer *clRenderer = (CLRenderer *)renderer.get();
		clRenderer->Clear(0x404040);
		memset(pixels, 0, nPixels * sizeof(COLORREF));
	}

	void Window::Render()
	{
		if (!alive)
			return;
		BitBlt(dc, 0, 0, width, height, bmpDc, 0, 0, SRCCOPY);
	}

	// Benchmark function (ms)
	static double GetTime()
	{
		LARGE_INTEGER t, f;
		QueryPerformanceCounter(&t);
		QueryPerformanceFrequency(&f);
		return ((double)t.QuadPart / (double)f.QuadPart) * 1000;
	}

	void Window::Render(const std::unique_ptr<Renderer> &renderer)
	{
		if (!alive)
			return;

		CLRenderer *clRenderer = (CLRenderer *)renderer.get();
		if (clRenderer->IsDrawing())
		{
			using namespace std::chrono;
			double beg = GetTime();
			clRenderer->Draw();
			double end = GetTime();
			totalRenders++;
			totalRenderTime += end - beg;
		}
		Render();
	}

	void Window::Destroy()
	{
		if (!destroyed)
		{
			alive = false;
			ReleaseDC(handle, dc);
			DestroyWindow(handle);
			UnregisterClassW(className, module);

			printf("Render time (average) is %fms\n", totalRenderTime / totalRenders);

			destroyed = true;
		}
	}

	bool Window::IsAlive()
	{ return alive; }

	LONG Window::GetX()
	{ return x; }

	LONG Window::GetY()
	{ return y; }

	LONG Window::GetWidth()
	{ return width; }

	LONG Window::GetHeight()
	{ return height; }


	COLORREF *Window::GetPixels()
	{ return pixels; }

	LONG Window::GetPixelCount()
	{ return nPixels; }
}