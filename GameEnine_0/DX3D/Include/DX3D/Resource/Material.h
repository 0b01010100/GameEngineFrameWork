#pragma once
#include <DX3D/Resource/Resource.h>
#include <vector>


class GraphicsEngine;
class Material : public Resource
{
public:
	Material(const wchar_t* path, ResourceManager* manager);
	Material(const MaterialPtr& material, ResourceManager* manager );

	void addTexture(const TexturePtr& texture);
	void removeTexture(unsigned int index);
	void setData(void* data, unsigned int size);

	void setCullMode( const CullMode& mode) ;
	const CullMode getCullMode() ;
private:
	VertexShaderPtr m_vertex_shader = __nullptr;
	PixelShaderPtr m_pixel_shader = __nullptr;
	ConstantBufferPtr m_constant_buffer = __nullptr;
	std::vector<Texture2DPtr> m_vec_textures;
	CullMode m_cull_mode = CullMode::Back;
private:
	friend class GraphicsEngine;
};

