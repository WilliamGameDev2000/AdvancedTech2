#include "VertexBinder.h"

VertexBinder::VertexBinder(Graphics& Gfx, std::vector<vertex> vertices)
	: stride (sizeof(vertex))
{
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(vertex);

	sd.pSysMem = vertices.data();

	GetDevice(Gfx)->CreateBuffer(&bd, &sd, pVertexBuffer.GetAddressOf());
}

void VertexBinder::SetBuffer(Graphics& Gfx)
{
	const UINT offset = 0u;
	GetContext(Gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

ID3D11DeviceContext* VertexBinder::GetContext(Graphics& Gfx) noexcept
{
	Gfx
}