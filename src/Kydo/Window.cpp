#include "Kydo/Window.h"

#include "Kydo/CLRenderer.h"

namespace Kydo
{
	Window *Window::Instance;

	LRESULT CALLBACK Window::KydoWinProc(__in HWND wnd, __in UINT msg, __in WPARAM wParam, __in LPARAM lParam)
	{
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

		BITMAPINFO bmpInfo = { { sizeof(BITMAPINFO) } };
		bmpInfo.bmiHeader.biWidth = width;
		bmpInfo.bmiHeader.biHeight = height;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = sizeof(COLORREF) * 8;
		bmpInfo.bmiHeader.biCompression = BI_RGB;

		dc = GetDC(handle);
		HBITMAP bmp = CreateDIBSection(dc, &bmpInfo, DIB_RGB_COLORS, (void **)&pixels, NULL, 0);
		assert(bmp);
		bmpDc = CreateCompatibleDC(dc);
		assert(bmpDc);
		SelectObject(bmpDc, bmp);
	}

	Window::~Window()
	{ Destroy(); }


	void Window::Show()
	{ ShowWindowAsync(handle, TRUE); }

	void Window::Hide()
	{ ShowWindowAsync(handle, FALSE); }


	void Window::Update()
	{
		if (PeekMessageW(&msg, handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window::Clear()
	{
		std::printf("%i\n", nPixels);
		memset(pixels, 0, nPixels * sizeof(COLORREF));
	}

	void Window::Render()
	{ RedrawWindow(handle, NULL, NULL, RDW_INVALIDATE); }

	void Window::Render(const std::unique_ptr<Renderer> &renderer)
	{
		CLRenderer *clRenderer = (CLRenderer *)renderer.get();
		if (clRenderer->IsDrawing())
			clRenderer->Draw();
		Render();
	}

	void Window::Destroy()
	{
		if (!destroyed)
		{
			ReleaseDC(handle, dc);
			DestroyWindow(handle);
			UnregisterClassW(className, module);

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