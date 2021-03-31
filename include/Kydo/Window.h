#pragma once

#pragma region include <Windows.h>
#define STRICT

#define NOGDICAPMASKS	  // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOSYSMETRICS	  // SM_*
#define NOMENUS			  // MF_*
#define NOICONS			  // IDI_*
#define NOKEYSTATES		  // MK_*
#define NOSYSCOMMANDS	  // SC_*
#define NORASTEROPS		  // Binary and Tertiary raster ops
#define OEMRESOURCE		  // OEM Resource values
#define NOATOM			  // Atom Manager routines
#define NOCLIPBOARD		  // Clipboard routines
#define NOCOLOR			  // Screen colors
#define NOCTLMGR		  // Control and Dialog routines
#define NODRAWTEXT		  // DrawText() and DT_*
#define NOGDI			  // All GDI defines and routines
#define NOKERNEL		  // All KERNEL defines and routines
#define NOMEMMGR		  // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE		  // typedef METAFILEPICT
#define NOMINMAX		  // Macros min(a,b) and max(a,b)
#define NOOPENFILE		  // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL		  // SB_* and scrolling routines
#define NOSERVICE		  // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND			  // Sound driver routines
#define NOTEXTMETRIC	  // typedef TEXTMETRIC and associated routines
#define NOWH			  // SetWindowsHook and WH_*
#define NOWINOFFSETS	  // GWL_*, GCL_*, associated routines
#define NOCOMM			  // COMM driver routines
#define NOKANJI			  // Kanji support stuff.
#define NOHELP			  // Help engine interface.
#define NOPROFILER		  // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX			  // Modem Configuration Extensions
#include <Windows.h>
#pragma endregion
#include "Kydo/Core.h"

namespace Kydo
{
	KYDO_API class Window
	{
	private:
		MSG msg;
		HWND handle;
		HMODULE module;

		PCWSTR className, title;
		WNDCLASSEXW clazz;

		RECT rect;

		bool alive = true, destroyed = false;
	public:
		Window(PCWSTR title, LONG width = 1280, LONG height = 720, PCWSTR className = L"MyClass", HMODULE = NULL);
		~Window() { Destroy(); }

		void Show();
		void Hide();

		void Update();
		void Destroy();

		bool IsAlive();
	};
}