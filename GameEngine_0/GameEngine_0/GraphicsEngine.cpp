#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>
GraphicsEngine * GraphicsEngine::m_engine = nullptr;
GraphicsEngine::GraphicsEngine()
{
	try
	{
		m_render_system = new RenderSystem();
	}
	catch (const std::exception&){ throw std::exception("RenderSystem was not created Successfully");}

	try
	{
		m_tex_manager = new TextureManager();
	}
	catch (const std::exception&){ throw std::exception("TextureManager was not created Successfully"); }
}

GraphicsEngine::~GraphicsEngine()
{
	
	GraphicsEngine::m_engine = nullptr;
	delete m_tex_manager;
	delete m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{

	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_tex_manager;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine) throw std::exception("GraphicsEngine was already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine) return;
	delete GraphicsEngine::m_engine;
}

GraphicsEngine * GraphicsEngine::get()
{
	return m_engine;
}