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

void GraphicsEngine::update ( const MeshData& data)
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

	constData.world.setRotationY ( 0.707f );

	constData.view.setTranslation ( Vector3D ( 0.f, 0.f, -10.0f ) );
	constData.view.inverse ( );

	constData.proj.setPerspectiveFovLH ( 1.3f, (float)winSize.width / (float)winSize.height, 0.01f, 1000.0f );



	context->setVertexBuffer ( data.mesh->m_vertex_buffer );
	context->setIndexBuffer ( data.mesh->m_index_buffer );

	for (auto i = 0; i < data.mesh->getNumMaterialSlots ( ); i++) 
	{
		data.material->setData ( &constData, sizeof ( ConstantData ) );
		context->setConstantBuffer ( data.material->m_constant_buffer );

		context->setVertexShader ( data.material->m_vertex_shader );
		context->setPixelShader ( data.material->m_pixel_shader );

		context->setTexture ( &data.material->m_vec_textures[0], data.material->m_vec_textures.size());

		auto slot = data.mesh->getMaterialSlot ( i );
		context->drawIndexedTriangleList ( slot.num_indices, slot.start_index, 0 );
	}

	swapChain->present ( 1 );
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system.get ( );
}

GraphicsEngine::~GraphicsEngine ( )
{


}