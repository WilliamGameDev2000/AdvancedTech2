#include "Window.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

//Window class definition
Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = MessageInit;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInstance;
}

//Window definition
Window::Window(int width, int height, int xPos, int yPos, const char* name) noexcept
	: _width(width), _height(height)
{
	const int offset = 100;
	RECT wr;
	wr.left = offset;
	wr.right = _width + wr.left;
	wr.top = offset;
	wr.bottom = _height + wr.top;

	hWnd = CreateWindow(WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, xPos, yPos,
		wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow(hWnd, SW_SHOW);

	gfxPtr = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		return;
	}
}

std::optional<int> Window::ProcessMessge()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}

	return{};
}

Graphics& Window::Gfx()
{
	return *gfxPtr;
}

LRESULT CALLBACK Window::MessageInit(HWND hWND, UINT msg, WPARAM wParam, LPARAM lparam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		SetWindowLongPtr(hWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::RuntimeMessageHandler));

		return pWnd->MessageHandler(hWND, msg, wParam, lparam);
	}

	return DefWindowProc(hWND, msg, wParam, lparam);
}

LRESULT CALLBACK Window::RuntimeMessageHandler(HWND hWND, UINT msg, WPARAM wParam, LPARAM lparam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWND, GWLP_USERDATA));

	return pWnd->MessageHandler(hWND, msg, wParam, lparam);
}

LRESULT Window::MessageHandler(HWND hWND, UINT msg, WPARAM wParam, LPARAM lparam) noexcept
{
	switch (msg)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN:
		{
			KB.OnKeyPressed(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_KEYUP:
		{
			KB.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_CHAR:
		{
			KB.OnChar(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			mouse.OnMouseMove(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt MAKEPOINTS(lparam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt MAKEPOINTS(lparam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt MAKEPOINTS(lparam);
			mouse.OnLeftReleased(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt MAKEPOINTS(lparam);
			mouse.OnRightReleased(pt.x, pt.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse.OnWheelUp(pt.x, pt.y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				mouse.OnWheelDown(pt.x, pt.y);
			}
			break;
		}
	}

	return DefWindowProc(hWND, msg, wParam, lparam);
}
