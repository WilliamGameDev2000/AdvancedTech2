#pragma once
#include "WillWin.h"
#include <d3d11.h>
#include <wrl.h>
#include <sstream>

class Graphics
{
public:
	Graphics( HWND hWnd);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTriangle(float angle, float x, float z);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
};