#include "TextureSampler.h"

TextureSampler::TextureSampler(Graphics& renderer)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	GetDevice(renderer)->CreateSamplerState(&samplerDesc, &sampler);
}

void TextureSampler::Bind(Graphics& renderer) noexcept
{
	GetContext(renderer)->PSSetSamplers(0U, 1U, sampler.GetAddressOf());
}