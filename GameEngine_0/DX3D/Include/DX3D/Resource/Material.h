#pragma once
#include <DX3D/Resource/Resource.h>
#include <vector>

enum CULL_MODE
{
	CULL_MODE_FRONT = 0b0,
	CULL_MODE_BACK = 0x1
};
class GraphicsEngine;
class Material : public Resource
{
public:
	Material(const wchar_t* path, ResourceManager* manager);
	Material(const MaterialPtr& material, ResourceManager* manager );

	void addTexture(const TexturePtr& texture);
	void removeTexture(unsigned int index);
	void setData(void* data, unsigned int size);

	void setCullMode(CULL_MODE mode) ;
	const CULL_MODE getCullMode() ;
private:
	VertexShaderPtr m_vertex_shader = __nullptr;
	PixelShaderPtr m_pixel_shader = __nullptr;
	ConstantBufferPtr m_constant_buffer = __nullptr;
	std::vector<Texture2DPtr> m_vec_textures;
	CULL_MODE m_cull_mode = {};
private:
	friend class GraphicsEngine;
};

