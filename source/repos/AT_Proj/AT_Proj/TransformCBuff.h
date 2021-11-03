#pragma once
#include "ConstantBuffer.h"
#include "Object.h"
#include <DirectXMath.h>

class TransformCbuff : public Bindable
{
public:
	TransformCbuff(Graphics& gfx, const Object& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf;
	const Object& parent;
};