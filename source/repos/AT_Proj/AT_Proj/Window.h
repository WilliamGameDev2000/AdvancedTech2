#pragma once
#include "WillWin.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class Window
{
private:
	//singlton to handle register/clean up on window class
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		static constexpr const char* wndClassName = "AT Project Window";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, int xPos, int yPos, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessge();
	Graphics& Gfx();
private:
	static LRESULT CALLBACK MessageInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	static LRESULT CALLBACK RuntimeMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;

	std::unique_ptr<Graphics> gfxPtr;

	int width;
	int height;
	int xPos;
	int yPos;
	HWND hWnd;
};

#define CHWND_EXCEPT( Hres) Window::Exception( __LINE__,__FILE__,Hres)
#define CHWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError())