#include <DX3D/Resource/Texture.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>

Texture::Texture ( const wchar_t* full_path, ResourceManager* manager ) : Resource( full_path, manager)
{
	m_texture = manager->getGame ( )->getGraphicsEngine ( )->getRenderSystem ( )->createTexture ( full_path );
}

const Texture2DPtr& Texture::getTexture ( )
{
	return this->m_texture;
}
