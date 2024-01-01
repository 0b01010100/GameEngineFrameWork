/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2022, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

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

#include <DX3D/Entity/Entity.h>

__declspec(align(16))
struct LightData
{
	Vector4D color;
	Vector4D direction;
};

__declspec(align(16))
struct ConstantData
{
	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 proj;
	Vector4D cameraPosition;
	LightData light;
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
}

void GraphicsEngine::removeComponent ( Component* component )
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.erase ( c );
	else if (auto c = dynamic_cast<CameraComponent*>(component))
		m_cameras.erase ( c );
	else if (auto c = dynamic_cast<LightComponent*>(component))
		m_lights.erase ( c );
}

GraphicsEngine::~GraphicsEngine ( )
{
}
