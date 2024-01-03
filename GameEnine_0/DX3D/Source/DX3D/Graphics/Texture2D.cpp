#include <DX3D/Graphics/Texture2D.h>
#include <DirectXTex.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>

Texture2D::Texture2D (const wchar_t* full_path, RenderSystem* system ): m_system(system)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);

	if (SUCCEEDED(res))
	{
		
		res = DirectX::CreateTexture( m_system->m_d3d_device.Get(), image_data.GetImages ( ),
			image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);
		if (FAILED(res)) DX3DError("Texture not created successfully");
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = (ui32)image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;



		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV= D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW= D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = (ui32)image_data.GetMetadata().mipLevels;

		res = m_system->m_d3d_device.Get ( )->CreateSamplerState(&sampler_desc, &m_sampler_state);
		if (FAILED(res)) DX3DError("Texture not created successfully");

		res = m_system->m_d3d_device.Get ( )->CreateShaderResourceView ( this->m_texture.Get(), &desc,
			&m_shader_res_view );
	}
	else
	{
		DX3DError("Texture not created successfully");
	}
	m_size = Rect ( 0, 0, (i32)image_data.GetMetadata ( ).width, (i32)image_data.GetMetadata ( ).height );
}

Texture2D::Texture2D (const Rect& size, Texture2D::Type type, RenderSystem* system ) : m_system(system)
{
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = size.width;
	tex_desc.Height = size.height;

	if (type == Texture2D::Type::Normal) {
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE/*Allows us to use the texture inside shaders*/;
	}
	else if (type == Texture2D::Type::RenderTarget) {
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture2D::Type::DepthStencil) {
		tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}

	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	HRESULT hr = m_system->m_d3d_device.Get ( )->CreateTexture2D(&tex_desc, 0, (ID3D11Texture2D**)m_texture.GetAddressOf() );


	if (FAILED(hr))
	{
		DX3DError("Texture not Created successfully");
	}
	if (type == Texture2D::Type::Normal || type == Texture2D::Type::RenderTarget) {
		hr = m_system->m_d3d_device.Get ( )->CreateShaderResourceView(m_texture.Get ( ), NULL, &m_shader_res_view);
		if (FAILED(hr))
		{
			DX3DError("SwapChain not Created successfully");
		}
	}
	if (type == Texture2D::Type::RenderTarget) {
		hr = m_system->m_d3d_device.Get ( )->CreateRenderTargetView(m_texture.Get ( ), NULL, &m_render_target_view);
		if (FAILED(hr))
		{
			DX3DError("SwapChain not Created successfully");
		}
	}
	else if (type == Texture2D::Type::DepthStencil) {
		hr = m_system->m_d3d_device.Get ( )->CreateDepthStencilView(m_texture.Get(), NULL, &m_depth_stencil_view );
		if (FAILED(hr))
		{
			DX3DError("SwapChain not Created successfully");
		}
	}

	m_type = type;
	m_size = size;
}




Rect Texture2D::getSize()
{
	return m_size;
}

Texture2D::Type Texture2D::getType()
{
	return m_type;
}
