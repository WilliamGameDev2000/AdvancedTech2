#include "Graphics.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib,"D3DCompiler.lib")

#pragma comment (lib, "d3d11.lib")
namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

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

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSSTATE;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSSTATE);

	pContext->OMSetDepthStencilState(pDSSTATE.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = 1024u;
	depthDesc.Height = 720u;
	depthDesc.MipLevels = 1u;
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1u;
	depthDesc.SampleDesc.Quality = 0u;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

}


void Graphics::EndFrame()
{
	pChain->Present(1u,0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float colour[] = { red, green, blue, 1.0F };
	pContext->ClearRenderTargetView(pTarget.Get() , colour);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
/* TODO Pass translation matrix into draw function for upadating constant buffer,
		Separate 3dprimitives into own classes,
		Move Vertex buffer/Pixel buffer out into single creation functions/classes,
		Move Viewport into separate single creation function,
		**NEW**
		Create Camera Class,
		Create Level Loader,

		*/
void Graphics::DrawCube(float angle, float x, float z)
{

	struct vertex 
	{
		struct {
			float x;
			float y;
			float z;
		}position;
		
	};

	vertex vertecies[] =
	{
		{-1.0f, -1.0f, -1.0f}, //0 back bottom left
		{1.0f, -1.0f, -1.0f},  //1 back bottom right
		{-1.0f, 1.0f, -1.0f},  //2 back top left
		{1.0f, 1.0f, -1.0f},   //3 back top right
		{-1.0f, -1.0f, 1.0f},  //4 front bottom left
		{1.0f, -1.0f, 1.0f},   //5 front bottom right
		{-1.0f, 1.0f, 1.0f},   //6 front top left
		{1.0f, 1.0f, 1.0f},    //7 front top right
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
		0,2,1, 2,3,1, //back face
		1,3,5, 3,7,5, //right face
		2,6,3, 3,6,7, //top face
		4,5,7, 4,7,6, //front face
		0,4,2, 2,4,6, //left face
		0,1,4, 1,5,4, //bottom face

		//USE FOR WIREFRAME MODELS
		/*0,1, 0,2, 0,3, 0,4, 0,5, 0,6,
		1,2, 1,3, 1,5, 1,7,
		2,3, 2,4, 2,6, 2,7,
		3,5, 3,6, 3,7,
		4,5, 4,6, 4,7,
		5,7,
		6,7*/
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
		dx::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle) *
			dx::XMMatrixRotationX(angle) *
			dx::XMMatrixTranslation(x, 0.0f,z + 4.0f)*
			dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f))
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

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		}face_colours[6];
	};

	const ConstantBuffer2 cb2
	{
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);

	pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());
	
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
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	pDevice->CreateInputLayout(ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), &pInputLayout);

	pContext->IASetInputLayout(pInputLayout.Get());


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
