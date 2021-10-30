#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "Graphics.h"
#include <vector>
#include "Vertex.h"


class VertexBinder
{
public:
	struct {
		float x;
		float y;
		float z;
	} position;
	VertexBinder(Graphics& Gfx,std::vector<vertex> vertices);
	~VertexBinder();
	void Bind();
	void SetBuffer(Graphics& Gfx);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA sd = {};
	static ID3D11DeviceContext* GetContext(Graphics& Gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& Gfx) noexcept;
	UINT stride;
};
