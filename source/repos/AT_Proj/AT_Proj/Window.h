#pragma once
#include "WillWin.h"
#include "Graphics.h"
#include <optional>
#include <memory>
#include "Mouse.h"
#include "Keyboard.h"

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
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessge();
	Graphics& Gfx();
	Mouse mouse;
	Keyboard KB;

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