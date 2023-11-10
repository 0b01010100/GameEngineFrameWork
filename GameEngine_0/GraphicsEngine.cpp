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
	catch (const std::exception&)
	{
		throw std::exception("Graphics was not created Successfully");
	}
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{

	return m_render_system;
}

GraphicsEngine * GraphicsEngine::get()
{
	return m_engine;
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
