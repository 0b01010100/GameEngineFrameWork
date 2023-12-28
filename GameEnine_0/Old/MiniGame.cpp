#include "MiniGame.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "MathUtils.h"
#include <time.h>


struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	Vector4D m_light_position = Vector4D(0, 1, 0, 0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;

};


MiniGame::MiniGame()
{
}


void MiniGame::render()
{
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_render_target,
		0, 0.3f, 0.4f, 1);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(this->m_depth_stencil);


	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(m_render_target, m_depth_stencil);


	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	Rect view_size = m_render_target->getSize();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(view_size.width, view_size.height);


	//RENDER SPACESHIP
	m_list_materials.clear();
	m_list_materials.push_back(m_spaceship_mat);
	updateModel(m_current_spaceship_pos, m_current_spaceship_rot, Vector3D(1, 1, 1), m_list_materials);
	drawMesh(m_spaceship_mesh, m_list_materials);



	//RENDER ASTEROID	
	m_list_materials.clear();
	m_list_materials.push_back(m_asteroid_mat);
	for (unsigned int i = 0; i < 200; i++)
	{
		updateModel(m_asteroids_pos[i], m_asteroids_rot[i], m_asteroids_scale[i], m_list_materials);
		drawMesh(m_asteroid_mesh, m_list_materials);
	}



	//RENDER SKYBOX/SPHERE
	m_list_materials.clear();
	m_list_materials.push_back(m_sky_mat);
	drawMesh(m_sky_mesh, m_list_materials);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = 0.0166666666f;// (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_time += m_delta_time;
}

void MiniGame::update()
{
	updateSpaceship();
	updateThirdPersonCamera();
	updateLight();
	updateSkyBox();
}

void MiniGame::updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials)
{
	constant cc;

	Matrix4x4 temp;
	cc.m_world.setIdentity();

	temp.setIdentity();
	temp.setScale(scale);
	cc.m_world *= temp;


	temp.setIdentity();
	temp.setRotationX(rotation.m_x);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(rotation.m_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationZ(rotation.m_z);
	cc.m_world *= temp;


	temp.setIdentity();
	temp.setTranslation(position);
	cc.m_world *= temp;


	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	cc.m_light_position = m_light_position;
	cc.m_light_radius = 0.0f;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;

	for (unsigned int m = 0; m < list_materials.size(); m++)
	{
		list_materials[m]->setData(&cc, sizeof(constant));
	}
}

void MiniGame::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_cam_rot.m_x += m_delta_mouse_y * 0.1f;
	m_cam_rot.m_y += m_delta_mouse_x * 0.1f;

	temp.setIdentity();
	temp.setRotationX(m_cam_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_cam_rot.m_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.05f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.05f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	updateViewportProjection();
}

void MiniGame::updateThirdPersonCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_cam_rot.m_x += m_delta_mouse_y * 0.001f;
	m_cam_rot.m_y += m_delta_mouse_x * 0.001f;


	if (m_cam_rot.m_x >= 1.57f)
		m_cam_rot.m_x = 1.57f;
	else if (m_cam_rot.m_x <= -1.57f)
		m_cam_rot.m_x = -1.57f;


	m_current_cam_rot = Vector3D::lerp(m_current_cam_rot, m_cam_rot, 3.0f * m_delta_time);

	temp.setIdentity();
	temp.setRotationX(m_current_cam_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_current_cam_rot.m_y);
	world_cam *= temp;

	if (m_forward)
	{
		if (m_turbo_mode)
		{
			if (m_forward > 0.0f) m_cam_distance = 25.0f;
			else m_cam_distance = 5.0f;
		}
		else
		{
			if (m_forward > 0.0f) m_cam_distance = 16.0f;
			else m_cam_distance = 9.0f;
		}
	}
	else
	{
		m_cam_distance = 14.0f;
	}

	m_current_cam_distance = lerp(m_current_cam_distance, m_cam_distance, 2.0f * m_delta_time);



	m_cam_pos = m_current_spaceship_pos;


	Vector3D new_pos = m_cam_pos + world_cam.getZDirection() * (-m_current_cam_distance);
	new_pos = new_pos + world_cam.getYDirection() * (5.0f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	updateViewportProjection();
}

void MiniGame::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(4000.0f, 4000.0f, 4000.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_mat->setData(&cc, sizeof(constant));
}

void MiniGame::updateLight()
{
	Matrix4x4 temp;
	m_light_rot_matrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(-0.707f);
	m_light_rot_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(0.707f);
	m_light_rot_matrix *= temp;

}

void MiniGame::updateSpaceship()
{
	Matrix4x4 world_model, temp;
	world_model.setIdentity();

	m_spaceship_rot.m_x += m_delta_mouse_y * 0.001f;
	m_spaceship_rot.m_y += m_delta_mouse_x * 0.001f;


	if (m_spaceship_rot.m_x >= 1.57f)
		m_spaceship_rot.m_x = 1.57f;
	else if (m_spaceship_rot.m_x <= -1.57f)
		m_spaceship_rot.m_x = -1.57f;


	m_current_spaceship_rot = Vector3D::lerp(m_current_spaceship_rot, m_spaceship_rot, 5.0f * m_delta_time);

	temp.setIdentity();
	temp.setRotationX(m_current_spaceship_rot.m_x);
	world_model *= temp;

	temp.setIdentity();
	temp.setRotationY(m_current_spaceship_rot.m_y);
	world_model *= temp;


	m_spaceship_speed = 125.0f;

	if (m_turbo_mode)
		m_spaceship_speed = 3005.0f;

	m_spaceship_pos = m_spaceship_pos + world_model.getZDirection() * (m_forward)*m_spaceship_speed * m_delta_time;
	m_current_spaceship_pos = Vector3D::lerp(m_current_spaceship_pos, m_spaceship_pos, 3.0f * m_delta_time);

}

void MiniGame::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{
	for (unsigned int m = 0; m < list_materials.size(); m++)
	{
		if (m == list_materials.size()) break;

		MaterialSlot mat = mesh->getMaterialSlot(m);

		GraphicsEngine::get()->setMaterial(list_materials[m]);
		//SET THE VERTICES OF THE TRIANGLE TO DRAW
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
		//SET THE INDICES OF THE TRIANGLE TO DRAW
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());
		// FINALLY DRAW THE TRIANGLE
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList((UINT)mat.num_indices, 0, (UINT)mat.start_index);
	}
}

void MiniGame::updateViewportProjection()
{
}


MiniGame::~MiniGame()
{
}



TexturePtr& MiniGame::getRenderTarget()
{
	return m_render_target;
}

void MiniGame::setWindowSize(const Rect& win_size)
{
	this->win_size = win_size;


}

void MiniGame::onCreate()
{
	m_play_state = true;

	srand((unsigned int)time(NULL));


	for (unsigned int i = 0; i < 200; i++)
	{
		m_asteroids_pos[i] = Vector3D((rand() % 4000) + (-2000.0f), (rand() % 4000) + (-2000.0f), (rand() % 4000) + (-2000.0f));
		m_asteroids_rot[i] = Vector3D((rand() % 628) / 100.0f, (rand() % 628) / 100.0f, (rand() % 628) / 100.0f);
		float scale = rand() % 20 + (6.0f);
		m_asteroids_scale[i] = Vector3D(scale, scale, scale);
	}


	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\stars_map.jpg");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\sphere.obj");


	m_spaceship_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\spaceship.jpg");
	m_spaceship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\spaceship.obj");

	m_asteroid_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\asteroid.jpg");
	m_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\asteroid.obj");


	m_base_mat = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	m_base_mat->setCullMode(CULL_MODE_BACK);

	m_spaceship_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_spaceship_mat->addTexture(m_spaceship_tex);
	m_spaceship_mat->setCullMode(CULL_MODE_BACK);

	m_asteroid_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_asteroid_mat->addTexture(m_asteroid_tex);
	m_asteroid_mat->setCullMode(CULL_MODE_BACK);

	m_sky_mat = GraphicsEngine::get()->createMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	m_sky_mat->addTexture(m_sky_tex);
	m_sky_mat->setCullMode(CULL_MODE_FRONT);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	m_list_materials.reserve(32);


	m_render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(1280, 720), Texture::Type::RenderTarget);
	m_depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(1280, 720), Texture::Type::DepthStencil);

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)1280 / (float)720), 0.1f, 5000.0f);
}

void MiniGame::onUpdate()
{
	this->update();
	this->render();

	m_delta_mouse_x = 0;
	m_delta_mouse_y = 0;
}

void MiniGame::onDestroy()
{
}

void MiniGame::onFocus()
{
}

void MiniGame::onKillFocus()
{
}

void MiniGame::onSize()
{
}

void MiniGame::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
	else if (key == VK_SHIFT)
	{
		m_turbo_mode = true;
	}

}

void MiniGame::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == VK_SHIFT)
	{
		m_turbo_mode = false;
	}

}

void MiniGame::onMouseMove(const Point& mouse_pos)
{
	int width = (win_size.width);
	int height = (win_size.height);

	m_delta_mouse_x = (float)(mouse_pos.m_x - (win_size.left + (width / 2.0f)));
	m_delta_mouse_y = (float)(mouse_pos.m_y - (win_size.top + (height / 2.0f)));
}

void MiniGame::onLeftMouseDown(const Point& mouse_pos)
{
}

void MiniGame::onLeftMouseUp(const Point& mouse_pos)
{
}

void MiniGame::onRightMouseDown(const Point& mouse_pos)
{
}

void MiniGame::onRightMouseUp(const Point& mouse_pos)
{
}
