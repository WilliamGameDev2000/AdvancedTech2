#pragma once
#include "ObjectDrawable.h"
//#include "Collision.h"
//#include "Textures.h"

class Plane : public ObjectDrawable<Plane>
{
public:
	Plane(Graphics& gfx);
	void Update(float dt) noexcept override;
	void setPos(float xpos, float ypos, float zpos);
	void setRot(XMFLOAT3 look_pos);
	//DirectX::XMFLOAT3 GetPos();
	//bool isColliding(DirectX::XMFLOAT3 pos1, float x_size, float y_size, float z_size);
	DirectX::XMMATRIX getRot() const;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void LoadTexture() noexcept override;
private:
	// positional
	DirectX::XMFLOAT3 pos = { 0,0,0 };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f };
	XMMATRIX look_at;
	//Collision collider;
	//Textures texture;
};
