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
			if (Instance->pixels && Instance->renderer)
			{
				//memcpy(Instance->pixels, ((CLRenderer *)Instance->renderer)->Pixels, Instance->width * Instance->height * sizeof(COLORREF));

				//memset(Instance->pixels, 0xFF, Instance->width * Instance->height * sizeof(COLORREF));

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
		if (GetMessageW(&msg, handle, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window::Render()
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(50ms);
		SwapBuffers(dc);
	}

	void Window::Render(const std::unique_ptr<Renderer> &renderer)
	{
		CLRenderer *clRenderer = (CLRenderer *)renderer.get();
		if (clRenderer->IsDrawing())
		{
			this->renderer = clRenderer;
			clRenderer->Draw();
			Render();
		}
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