#include "Plane.h"
#include "BindableBase.h"


Plane::Plane(Graphics& gfx)
{
	if (!IsStaticInitialized())
	{
		const std::vector<Vertex> vertices =
		{
			{ -1.0f,-1.0f,0.0f },
			{ -1.0f,1.0f,0.0f },
			{ 1.0f,-1.0f,0.0f },
			{ 1.0f,1.0f,0.0f },
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

		const std::vector<unsigned short> indices =
		{
			0, 2, 1,
			2, 3, 1
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[1];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{ 0.5f,0.5f,0.5f }
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuff>(gfx, *this));
}

void Plane::Update(float dt) noexcept
{
	//rotate towards player
	//not quite done, need a new 
	//setPos(sin(objYaw), 0, cos(objYaw));
}

void Plane::setPos(float xpos, float ypos, float zpos)
{
	pos.x = xpos;
	pos.y = ypos;
	pos.z = zpos;
}

void Plane::setRot(XMFLOAT3 look_pos)
{
	XMVECTOR eye_vec = XMVectorSet(pos.x, pos.y, pos.z, 0);
	XMVECTOR target_vec = XMVectorSet(look_pos.x, look_pos.y, look_pos.z, 0);

	look_at = DirectX::XMMatrixLookAtRH(eye_vec, target_vec, up);
}

DirectX::XMMATRIX Plane::getRot() const
{
	return look_at;
}

DirectX::XMMATRIX Plane::GetTransformXM() const noexcept
{
	return getRot() *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Plane::LoadTexture() noexcept
{
}