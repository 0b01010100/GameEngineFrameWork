#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>
GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	m_render_system = new RenderSystem();
	m_render_system->init();
	return true;
}

//Realses the Resources 
bool GraphicsEngine::release()
{
	m_render_system->release();
	delete m_render_system;
	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

RenderSystem* GraphicsEngine::getRenderSystem()
{

	return m_render_system;
}

GraphicsEngine * GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
