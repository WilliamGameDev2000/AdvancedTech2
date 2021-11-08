#pragma once
#include "ObjectDrawable.h"

class Cube : public ObjectDrawable<Cube>
{
public:
	Cube(Graphics& gfx);
	void Update(float dt) noexcept override;
	void setPos(float xpos,float ypos,float zpos);
	void setPosX(float xpos);
	void setPosY(float ypos);
	void setPosZ(float zpos);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float xPos = 0;
	float yPos = 0;
	float zPos = 0;
};