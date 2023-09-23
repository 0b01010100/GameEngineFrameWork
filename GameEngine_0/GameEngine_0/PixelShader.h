#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
class GraphicsEngine;
class DeviceContext;
class PixelShader
{
	public:
		PixelShader();
		void release();
		~PixelShader();
	private:
		///Init function allows us to pass in the compiled shader code during initalizion to create a new shader
		bool init(const void* shader_byte_code, size_t byte_code_size);
	private:
		ID3D11PixelShader* m_ps;
	private:
		friend class GraphicsEngine;
		friend class DeviceContext;

};

