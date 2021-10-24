#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")

#pragma comment (lib, "d3d11.lib")
namespace wrl = Microsoft::WRL;
Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pChain,
		&pDevice,
		nullptr,
		&pContext);

	wrl::ComPtr<ID3D11Resource> BackBufferPtr = nullptr;
	pChain->GetBuffer(0, __uuidof(ID3D11Resource), &BackBufferPtr);
	pDevice->CreateRenderTargetView(
		BackBufferPtr.Get(),
		nullptr,
		&pTarget);
}


void Graphics::EndFrame()
{
	pChain->Present(1u,0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float colour[] = { red, green, blue, 1.0F };
	pContext->ClearRenderTargetView(pTarget.Get() , colour);
}

void Graphics::DrawTriangle(float angle)
{
	namespace wrl = Microsoft::WRL;

	struct vertex 
	{
		struct {
			float x;
			float y;
		}position;

		struct 
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char a;
		}colour;
		
	};
	//pass colours into the vertexs

	vertex vertecies[] =
	{
		/*{ Coords.first, Coords.second, 120,0,0,0},
		{ Coords2.first, Coords2.second, 255,0,0,0},
		{ Coords3.first, Coords3.second, 120,0,0,0},

		{ Coords2.first, Coords.second * 2, 0,0,255,0},
		{ Coords2.first * 2, Coords.second, 255,0,0,0},
		{ Coords2.first, Coords.second, 0,255,0,0}*/

		{0.0f,0.5f,255,0,0,0},
		{0.5f,-0.5f,0,255,0,0},
		{-0.5f,-0.5f,0,0,255,0},
		{-0.3f,0.3f,0,255,0,0},
		{0.3f,0.3f,0,0,255,0},
		{0.0f,-1.0f,255,0,0,0}
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertecies);
	bd.StructureByteStride = sizeof(vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertecies;

	pDevice->CreateBuffer(&bd, &sd,pVertexBuffer.GetAddressOf());

	const UINT stride = sizeof(vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	const unsigned short indicies[] =
	{
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5
	};

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indicies);
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indicies;

	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	struct ConstantBuffer
	{
		struct
		{
			float element[4][4];
		}transformation;
	};

	const ConstantBuffer cb =
	{
		{
			(3.0f / 4.0f) * std::cos(angle), std::sin(angle), 0.0f,0.0f,
			(3.0f / 4.0f) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
}
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	
	//create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		nullptr, &pPixelShader);
	//bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso",&pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		nullptr, &pVertexShader);
	//bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Colour", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0,8u, D3D11_INPUT_PER_VERTEX_DATA, 0}
		
	};
	pDevice->CreateInputLayout(ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), &pInputLayout);

	pContext->IASetInputLayout(pInputLayout.Get());

	//bind render target
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width = 1024;
	vp.Height = 720;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	pContext->DrawIndexed((UINT)std::size(indicies),0u, 0u);
}
