#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/Rect.h>
#include <d3d11.h>
#include <wrl.h>

class Texture2D{
public:

	enum class Type
	{
		Normal = 0x0,
		RenderTarget,
		DepthStencil
	};
	
public:
	Texture2D (const wchar_t* full_path, RenderSystem* system);
	Texture2D (const Rect& size, Texture2D::Type type, RenderSystem* system );
	Rect getSize();
	Texture2D::Type getType();
private:
	Microsoft::WRL::ComPtr< ID3D11Resource > m_texture = nullptr;
	Microsoft::WRL::ComPtr < ID3D11ShaderResourceView > m_shader_res_view = nullptr;
	Microsoft::WRL::ComPtr < ID3D11RenderTargetView > m_render_target_view = nullptr;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilView > m_depth_stencil_view = nullptr;
	Microsoft::WRL::ComPtr < ID3D11SamplerState > m_sampler_state = nullptr;

	Texture2D::Type m_type = Texture2D::Type::Normal;
	Rect m_size;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};