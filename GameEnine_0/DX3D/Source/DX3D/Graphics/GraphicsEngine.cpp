#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/SwapChain.h>

#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Resource/Material.h>

#include <DX3D/Game/Game.h>
#include <DX3D/Game/Display.h>

#include <DX3D/Math/Matrix4x4.h>

#include <DX3D/Entity/MeshComponent.h>
#include <DX3D/Entity/Entity.h>
#include <DX3D/Entity/TransformComponent.h>
struct alignas(16) ConstantData
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 proj;
};

GraphicsEngine::GraphicsEngine( Game* game ) : m_game( game )
{
	m_render_system = std::make_unique<RenderSystem> ( );
}

void GraphicsEngine::update ()
{
	auto swapChain = m_game->m_display->m_swapChain;

	auto context = m_render_system->getImmediateDeviceContext ( );

	context->clearRenderTargetColor ( swapChain, 1, 0, 0, 1 );
	auto winSize = m_game->m_display->getClientSize ( );
	context->setViewportSize ( winSize.width, winSize.height );
	ConstantData constData = {};

	constData.world.setIdentity ( );
	constData.view.setIdentity ( );
	constData.proj.setIdentity ( );

	

	constData.view.setTranslation ( Vector3D ( 0.f, 0.f, -10.0f ) );
	constData.view.inverse ( );

	constData.proj.setPerspectiveFovLH ( 1.3f, (float)winSize.width / (float)winSize.height, 0.01f, 1000.0f );

	for (auto m : m_meshes) {

		auto transform = m->getEntity ( )->getTransform ( );
		transform->getWorldMatrix ( constData .world);

		auto mesh = m->getMesh ( ).get ( );
		const auto materials = m->getMaterials ( );


		context->setVertexBuffer ( mesh->m_vertex_buffer );
		context->setIndexBuffer ( mesh->m_index_buffer );

		for (auto i = 0; i < mesh->getNumMaterialSlots ( ); i++)
		{
			if (i >= materials.size ( )) break;
			auto mat = materials[i].get ( );

			mat->setData ( &constData, sizeof ( ConstantData ) );
			context->setConstantBuffer ( mat->m_constant_buffer );

			context->setVertexShader ( mat->m_vertex_shader );
			context->setPixelShader ( mat->m_pixel_shader );

			context->setTexture ( &mat->m_vec_textures[0], (unsigned int)mat->m_vec_textures.size ( ) );

			auto slot = mesh->getMaterialSlot ( i );
			context->drawIndexedTriangleList ( (unsigned int)slot.num_indices, (unsigned int)slot.start_index, 0 );
		}
	}
	swapChain->present ( 1 );
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system.get ( );
}

void GraphicsEngine::addComponent ( Component* component )
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.emplace ( c );
}

void GraphicsEngine::removeComponent ( Component* component )
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		this->m_meshes.erase ( c );
}

GraphicsEngine::~GraphicsEngine ( )
{


}