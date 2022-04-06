#pragma once
#include "ObjectDrawable.h"
#include "Collision.h"

class Cube : public ObjectDrawable<Cube>
{
public:
	Cube(Graphics& gfx);
	~Cube() = default;
	void Update(float dt) noexcept override;
	void setPos(float xpos,float ypos,float zpos);
	void setScale(float xscale,float yscale,float zscale);
	DirectX::XMFLOAT3 GetPos();
	void setRot(float new_rot);
	void setPosX(float xpos);
	void setPosY(float ypos);
	void setPosZ(float zpos);
	void setBullet();
	bool isColliding(DirectX::XMFLOAT3 pos1, float x_size, float y_size, float z_size);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void LoadTexture() noexcept override;
private:
	// positional
	DirectX::XMFLOAT3 pos = {0,0,0};
	float objYaw = 0;
	DirectX::XMFLOAT3 scale = {1,1,1};
	DirectX::XMMATRIX rot = {};
	Collision collider;
	//Textures texture;
	bool is_bullet = false;
};