#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>

#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Resource/Material.h>

#include <DX3D/Game/Game.h>
#include <DX3D/Game/Display.h>

#include <DX3D/Math/Matrix4x4.h>
#include <DX3D/Math/Vector4D.h>

#include <DX3D/Entity/TransformComponent.h>
#include <DX3D/Entity/MeshComponent.h>
#include <DX3D/Entity/CameraComponent.h>
#include <DX3D/Entity/LightComponent.h>
#include <DX3D/Entity/TerrainComponent.h>

#include <DX3D/Entity/Entity.h>

__declspec(align(16))
struct LightData
{
	Vector4D color;
	Vector4D direction;
};
__declspec(align(16))
struct TerrainData
{
	Vector4D size;
	f32 heightMapSize = 0.0f;// the number of texels along one edge of the height map
};
__declspec(align(16))
struct ConstantData
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 proj;
	Vector4D cameraPosition;
	LightData light;
	TerrainData terrain;
};


GraphicsEngine::GraphicsEngine ( Game* game ) : m_game ( game )
{
	m_render_system = std::make_unique<RenderSystem> ( );
}

void GraphicsEngine::update ( )
{
	auto swapChain = m_game->m_display->m_swapChain;

	auto context = m_render_system->getImmediateDeviceContext ( );

	context->clearRenderTargetColor ( swapChain, 0, 0, 0, 1 );
	auto winSize = m_game->m_display->getClientSize ( );
	context->setViewportSize ( winSize.width, winSize.height );


	ConstantData constData = {};

	for (auto c : m_cameras)
	{
		auto t = c->getEntity ( )->getTransform ( );
		constData.cameraPosition = t->getPosition ( );
		c->setScreenArea ( winSize );
		c->getViewMatrix ( constData.view );
		c->getProjectionMatrix ( constData.proj );
	}


	for (auto l : m_lights)
	{
		auto t = l->getEntity ( )->getTransform ( );
		Matrix4x4 w;
		t->getWorldMatrix ( w );
		constData.light.direction = w.getZDirection ( );
		constData.light.color = l->getColor ( );
	}

	for (auto t : m_terrains) 
	{
		auto transform = t->getEntity ( )->getTransform ( );
		transform->getWorldMatrix ( constData.world );
		constData.terrain.size = t->getSize ( );
		constData.terrain.heightMapSize = t->getHeightMap ( )->getTexture ( )->getSize ( ).width;

		context->setVertexBuffer ( t->m_meshVb );
		context->setIndexBuffer ( t->m_meshIb );

		m_render_system->setCullMode ( CullMode::Back );
		t->updateData ( &constData, sizeof ( constData ) );
		context->setConstantBuffer ( t->m_cb );

		context->setVertexShader ( t->m_vertexShader );
		context->setPixelShader ( t->m_pixelShader );

		Texture2DPtr terrainTexture[3];
		terrainTexture[0] = t->getHeightMap ( )->getTexture ( );
		terrainTexture[1] = t->getGroundMap ( )->getTexture ( );
		terrainTexture[2] = t->getCliffMap ( )->getTexture ( );

		context->setTexture ( terrainTexture, 3 );

		context->drawIndexedTriangleList ( (ui32)t->m_meshIb->getSizeIndexList(), 0, 0);
	}

	for (auto m : m_meshes)
	{
		auto transform = m->getEntity ( )->getTransform ( );
		transform->getWorldMatrix ( constData.world );

		auto mesh = m->getMesh ( ).get ( );
		const auto materials = m->getMaterials ( );


		context->setVertexBuffer ( mesh->m_vertex_buffer );
		context->setIndexBuffer ( mesh->m_index_buffer );


		for (auto i = 0; i < mesh->getNumMaterialSlots ( ); i++)
		{
			if (i >= materials.size ( )) break;
			auto mat = materials[i].get ( );

			m_render_system->setCullMode ( mat->getCullMode ( ) );

			mat->setData ( &constData, sizeof ( ConstantData ) );
			context->setConstantBuffer ( mat->m_constant_buffer );

			context->setVertexShader ( mat->m_vertex_shader );
			context->setPixelShader ( mat->m_pixel_shader );

			context->setTexture ( &mat->m_vec_textures[0], (unsigned int)mat->m_vec_textures.size ( ) );

			auto slot = mesh->getMaterialSlot ( i );
			context->drawIndexedTriangleList ( (unsigned int)slot.num_indices, (unsigned int)slot.start_index, 0 );
		}
	}

	swapChain->present ( true );
}

RenderSystem* GraphicsEngine::getRenderSystem ( )
{
	return m_render_system.get ( );
}

void GraphicsEngine::addComponent ( Component* component )
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.emplace ( c );
	else if (auto c = dynamic_cast<CameraComponent*>(component))
	{
		if (!m_cameras.size ( )) m_cameras.emplace ( c );
	}
	else if (auto c = dynamic_cast<LightComponent*>(component))
	{
		if (!m_lights.size ( )) m_lights.emplace ( c );
	}
	else if (auto c = dynamic_cast<TerrainComponent*>(component))
	{
		if (!m_terrains.size ( )) m_terrains.emplace ( c );
	}
}

void GraphicsEngine::removeComponent ( Component* component )
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.erase ( c );
	else if (auto c = dynamic_cast<CameraComponent*>(component))
		m_cameras.erase ( c );
	else if (auto c = dynamic_cast<LightComponent*>(component))
		m_lights.erase ( c );
	else if (auto c = dynamic_cast<TerrainComponent*>(component))
	{
		m_terrains.erase ( c );
	}
}

GraphicsEngine::~GraphicsEngine ( )
{
}
