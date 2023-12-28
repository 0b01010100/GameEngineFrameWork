#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Input/InputSystem.h>


Game::Game ( )
{
	m_inputSystem = std::make_unique<  InputSystem > ( );
	m_graphicsEngine = std::make_unique < GraphicsEngine > ( this );
	m_display = std::make_unique < Display > ( this );
	m_resourceManager = std::make_unique < ResourceManager > ( this );


	m_mesh = m_resourceManager->createResourceFromFile<Mesh> ( L"Assests/Meshes/house.obj" );

	auto tex = m_resourceManager->createResourceFromFile<Texture> ( L"Assests/Textures/wood.jpg" );

	m_material = m_resourceManager->createResourceFromFile<Material> ( L"Assests/Shaders/MyMaterial.hlsl" );
	m_material->addTexture ( tex );

	m_inputSystem->SetlockArea ( m_display->getClientSize ( ) );
	m_inputSystem->lockCursor ( true );
}
Game::~Game ( ) {}
GraphicsEngine* Game::getGraphicsEngine ( )
{
	return this->m_graphicsEngine.get ( );
}
void Game::onDisplaySize ( const Rect& size )
{
	m_inputSystem->SetlockArea ( m_display->getClientSize ( ) );
	onInternalUpdate ( );

}
void Game::onInternalUpdate ( )
{
	m_inputSystem->update ( );
	
	if ( m_inputSystem->isKeyDown ( Key::Escape ) )
	{
		m_inputSystem->lockCursor ( false );
	}
	else if (m_inputSystem->isKeyDown ( Key::Enter ))
	{
		m_inputSystem->lockCursor ( true );
	}
	auto deltaPos = m_inputSystem->getDeltaMousePosition();
	DX3DInfo ( "Delta Mouse Pos: X:" << deltaPos.m_x << " Y: " << deltaPos.m_y );




	m_graphicsEngine->update ( { m_mesh,m_material } );
}
void Game::quit ( )
{
	m_isRunning = false;
}
