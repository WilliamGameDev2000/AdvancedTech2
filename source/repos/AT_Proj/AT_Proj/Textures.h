#pragma once
#include "Graphics.h"
#include <string>
#include "Bindable.h"

class Textures : public Bindable
{
public:
	Textures(Graphics& renderer, const std::wstring& filepath);
	void Bind(Graphics& renderer) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
};
