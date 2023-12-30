#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Game/World.h>


Game::Game ( )
{
	m_inputSystem = std::make_unique<  InputSystem > ( );
	m_graphicsEngine = std::make_unique < GraphicsEngine > ( this );
	m_display = std::make_unique < Display > ( this );
	m_resourceManager = std::make_unique < ResourceManager > ( this );
	m_world = std::make_unique < World > ( this );

	m_inputSystem->SetlockArea ( m_display->getClientSize ( ) );
}
Game::~Game ( ) {}
GraphicsEngine* Game::getGraphicsEngine ( )
{
	return this->m_graphicsEngine.get ( );
}
World* Game::getWorld ( )
{
	return m_world.get();
}
ResourceManager* Game::getResourceManager ( )
{
	return m_resourceManager.get();
}
void Game::onDisplaySize ( const Rect& size )
{
	m_inputSystem->SetlockArea ( m_display->getClientSize ( ) );
	onInternalUpdate ( );

}
void Game::onInternalUpdate ( )
{
	//COMPUTING DELTA TIME
	auto currentTime = std::chrono::system_clock::now ( );
	auto elapsedSeconds = std::chrono::duration<double> ( );//elapsed Time

	if (m_previousTime.time_since_epoch ( ).count ( ))//if m_previousTime is not zero do elapsedSeconds = currentTime - m_previousTime;
		elapsedSeconds = currentTime - m_previousTime;
	m_previousTime = currentTime;

	auto deltaTime = (f32)elapsedSeconds.count ( );


	m_inputSystem->update ( );

	onUpdate ( deltaTime );
	m_world->update ( deltaTime );

	m_graphicsEngine->update ( );
}
void Game::quit ( )
{
	m_isRunning = false;
}
