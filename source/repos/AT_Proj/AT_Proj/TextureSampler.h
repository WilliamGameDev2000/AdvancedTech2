#pragma once
#include "Graphics.h"
#include "Bindable.h"

class TextureSampler : public Bindable
{
public:
	TextureSampler(Graphics& renderer);
	void Bind(Graphics& renderer) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
};