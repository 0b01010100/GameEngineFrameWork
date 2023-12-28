#pragma once
//#include <d3d11.h> #include <wrl.h>
#include <DX3D/Prerequisites.h>
//#include <DX3D/Graphics/RenderSystem.h>
//#include "TextureManager.h"
//#include "MeshManager.h"
//#include "Material.h"

struct MeshData 
{
	MeshPtr mesh;
	MaterialPtr material;
};

class GraphicsEngine
{
public:
	//Initialize the GraphicsEngine and DirectX 11 Device
	GraphicsEngine(Game* game);
	//Release all the resources loaded
	~GraphicsEngine();
	

	void update ( const MeshData& data );

public:
	RenderSystem* getRenderSystem();
private:
	std::unique_ptr<RenderSystem> m_render_system = nullptr;
	Game* m_game = nullptr;
};

