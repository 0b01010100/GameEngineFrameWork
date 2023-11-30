#pragma once
#include"VertexShader.h"
#include"PixelShader.h"
#include "ConstantBuffer.h"
#include <vector>

enum CULL_MODE
{
	CULL_MODE_FRONT = 0b0,
	CULL_MODE_BACK = 0x1
};
class GraphicsEngine;
class Material
{
public:
	Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	Material(const MaterialPtr& material);
	~Material();


	void addTexture(const TexturePtr& texture);
	void removeTexture(unsigned int index);
	void setData(void* data, unsigned int size);

	void setCullMode(CULL_MODE mode) ;
	const CULL_MODE getCullMode() ;
private:
	VertexShaderPtr m_vertex_shader = __nullptr;
	PixelShaderPtr m_pixel_shader = __nullptr;
	ConstantBufferPtr m_constant_buffer = __nullptr;
	std::vector<TexturePtr> m_vec_textures;
	CULL_MODE m_cull_mode = {};
private:
	friend class GraphicsEngine;
};

