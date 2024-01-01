#include <DX3D/Resource/Material.h>
#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Resource/Texture.h>
Material::Material( const wchar_t* path, ResourceManager* manager ) : Resource( path, manager )
{
	auto rsys = manager->getGame ( )->getGraphicsEngine ( )->getRenderSystem ( );

	m_vertex_shader = rsys->createVertexShader( path, "vsmain" );
	if (not m_vertex_shader) DX3DError ("Material not created successfully");

	m_pixel_shader = rsys->createPixelShader( path, "psmain" );
	if (not m_pixel_shader) DX3DError ( "Material not created successfully" );

}

Material::Material ( const MaterialPtr& material, ResourceManager* manager ) : Resource ( L"", manager )
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
}

void Material::addTexture(const TexturePtr& texture)
{
	m_vec_textures.push_back(texture->m_texture);
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->m_vec_textures.size()) return;
	m_vec_textures.erase(m_vec_textures.begin() + index);
}

void Material::setData(void* data, unsigned int size)
{
	auto rsys = m_manager->getGame ( )->getGraphicsEngine ( )->getRenderSystem ( );
	if (!m_constant_buffer)
		m_constant_buffer = rsys->createConstantBuffer(data, size);
	else
		m_constant_buffer->update( rsys->getImmediateDeviceContext(), data);
}

void Material::setCullMode( const CullMode& mode)
{
	m_cull_mode = mode;
}

const CullMode Material::getCullMode()
{
	return m_cull_mode;
}
