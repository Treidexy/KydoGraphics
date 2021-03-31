#include "Kydo/Window.h"

namespace Kydo
{
	Window::Window(PCWSTR title, PCWSTR className, HMODULE module)
		: m_Module(module), m_Title(title), m_ClassName(className)
	{
		if (!m_Module)
			m_Module = GetModuleHandleW(NULL);

		m_Class = { sizeof(WNDCLASSEXW) };
		m_Class.hInstance = m_Module;
		m_Class.lpszClassName = m_ClassName;
		RegisterClassExW(&m_Class);

		RECT screen;
		GetWindowRect(GetDesktopWindow(), &screen);
		m_Handle = CreateWindowExW(0, m_ClassName, m_Title, WS_OVERLAPPEDWINDOW, (screen.right - m_Rect.right) >> 1, (screen.bottom - m_Rect.right) >> 1, m_Rect.right, m_Rect.bottom, NULL, NULL, m_Module, NULL);
	}
}