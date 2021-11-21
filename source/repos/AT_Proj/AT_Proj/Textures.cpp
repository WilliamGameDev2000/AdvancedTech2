#include "Textures.h"
#include "WICTextureLoader.h"

Textures::Textures(Graphics& renderer, const std::wstring& filepath)
{
	DirectX::CreateWICTextureFromFile(GetDevice(renderer), filepath.c_str(), NULL, &shader_resource_view);
}

void Textures::Bind(Graphics& renderer) noexcept
{
	GetContext(renderer)->PSSetShaderResources(0u, 1u, shader_resource_view.GetAddressOf());
}