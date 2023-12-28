#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/Rect.h>
class Game
{
public:
	Game ( );
	virtual ~Game ( );

	void run ( );
	void quit ( );

	GraphicsEngine* getGraphicsEngine ( );

protected:
	virtual void onCreate ( ) {}
	virtual void onUpdate ( float deltaTime ) {}
	virtual void onQuit ( ) {}
private:
	void onDisplaySize ( const Rect& size );
	void onInternalUpdate ( );
private:
	std::unique_ptr< InputSystem > m_inputSystem;
	std::unique_ptr< GraphicsEngine > m_graphicsEngine;
	std::unique_ptr< Display > m_display;
	std::unique_ptr<  ResourceManager > m_resourceManager;

	MeshPtr m_mesh;
	MaterialPtr m_material;

	bool m_isRunning = true;
private:
	friend class GraphicsEngine;
	friend class Display;
};