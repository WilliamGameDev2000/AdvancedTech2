#pragma once
#include "ObjectDrawable.h"
#include "Collision.h"

class Cube : public ObjectDrawable<Cube>
{
public:
	Cube(Graphics& gfx);
	void Update(float dt) noexcept override;
	void setPos(float xpos,float ypos,float zpos);
	DirectX::XMFLOAT3 GetPos();
	void setPosX(float xpos);
	void setPosY(float ypos);
	void setPosZ(float zpos);
	bool isColliding(DirectX::XMFLOAT3 pos1, float x_size, float y_size, float z_size);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	DirectX::XMFLOAT3 pos = {0,0,0};
	Collision collider;
};