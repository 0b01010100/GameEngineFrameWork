#include "SpaceShooterGame.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;//represents the transformation of an object in world space
	Matrix4x4 m_view;//camera's view transformation
	Matrix4x4 m_proj;//projection matrix used for perspective projection
	Vector4D m_light_direction;//Direction of the light normalised 
	Vector4D m_camera_position;//the camera's position
	Vector4D m_light_position = { 0, 1, 0, 0 };//The position of the point light
	float m_light_radius = 4.0f;//Radius of the point light
	float m_time = 0.0f;// delta time - time elasped from frame 1 to 2.
};//cbuffer


SpaceShooterGame::SpaceShooterGame()
{
}

void SpaceShooterGame::render()
{
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right, rc.bottom);



	//for (int i = 0; i < 3; i++)
	//{
	//	//RENDER torus_mesh
	//	updateModel(Vector3D(0, 2, -4 + 4 * i), m_mat);
	//	drawMesh(m_torus_mesh, m_mat);

	//	//RENDER sky_mesh
	//	updateModel(Vector3D(4, 2, -4 + 4 * i), m_earth_mat);
	//	drawMesh(m_sky_mesh, m_earth_mat);

	//	//RENDER suzanne_mesh
	//	updateModel(Vector3D(-4, 2, -4 + 4 * i), m_bricks_mat);
	//	drawMesh(m_suzanne_mesh, m_bricks_mat);
	//}

	//updateModel(Vector3D(0, 0, 0), m_mat);
	//drawMesh(m_plane_mesh, m_mat);

	//m_list_materials.clear();
	//m_list_materials.push_back(m_barrel_mat);
	//m_list_materials.push_back(m_brick_mat);
	//m_list_materials.push_back(m_windows_mat);
	//m_list_materials.push_back(m_wood_mat);
	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		updateModel(Vector3D(-14.0 + 14.0f * i, 0, -14.0 + 14.0f * j), m_list_materials);
	//		drawMesh(m_house_mesh, m_list_materials);
	//	}
	//}

	//m_list_materials.clear();
	//m_list_materials.push_back(m_terrain_mat);
	//updateModel(Vector3D(0, 0, 0), m_list_materials);
	//drawMesh(m_terrain_mesh, m_list_materials);
	

	//RENDER SPACESHIP
	m_list_materials.clear();
	m_list_materials.push_back(m_spaceship_mat);
	updateModel(m_spaceship_pos, m_spaceship_rot, Vector3D(1,1,1), m_list_materials);
	drawMesh(m_spaceship_mesh, m_list_materials);

	//RENDER ASTEROID
	m_list_materials.clear();
	m_list_materials.push_back(m_asteroid_mat);
	updateModel(Vector3D(1, 1, 20), Vector3D(0,0,0), Vector3D(1, 1, 1), m_list_materials);
	drawMesh(m_asteroid_mesh, m_list_materials);


	//SKY SPHERE
	m_list_materials.clear();
	m_list_materials.push_back(m_sky_mat);
	drawMesh(m_sky_mesh, m_list_materials);

	//DONE coloring on the render target now 
	//present it so it can be shown from the human eye.
	m_swap_chain->present(true);


	m_old_delta = m_new_delta;//Will soon be the time elapsed from the old frame to a new frame
	m_new_delta = ::GetTickCount();//Set the m_new_delta variable to current tick count

	// If m_old_delta is non-zero, this part calculates the time difference between the current frame 
	//m_new_delta and the previous frame m_old_delta. The difference is divided by 1000.0f, to convert it into seconds 
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;

	m_time += m_delta_time;//updating the program's notion of the current 
}

void SpaceShooterGame::update()//Update
{
	this->updateLight();
	this->updateSkyBox();
	this->updateSpaceShip();	
	this->updateThirdPersonCamera();
}

void SpaceShooterGame::updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials)
{
	constant cc = {};
	
	Matrix4x4 temp = {};
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
	cc.m_light_radius = m_light_radius;//Set the radius varaible equal to the radius varaible in the class 
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;
	for (size_t m = 0; m < list_materials.size(); m++)
	{
		list_materials[m]->setData(&cc, sizeof(constant));
	}
}

void SpaceShooterGame::updateCamera()
{

	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

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

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


	this->m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 600.0f);
	
}

void SpaceShooterGame::updateThirdPersonCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_cam_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_cam_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;



	if (m_cam_rot.m_x >= 1.57f)
		m_cam_rot.m_x = 1.57f;

	else if (m_cam_rot.m_x <= -1.57f)
		m_cam_rot.m_x = -1.57f;
	temp.setIdentity();
	temp.setRotationX(m_cam_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_cam_rot.m_y);
	world_cam *= temp;

	m_cam_pos = m_spaceship_pos;

	Vector3D new_pos = m_cam_pos + world_cam.getZDirection() * (-m_cam_distance);
	new_pos = new_pos + world_cam.getYDirection() * (5.0f);
	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


	this->m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void SpaceShooterGame::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_mat->setData(&cc, sizeof(constant));
}

void SpaceShooterGame::updateLight()
{
	Matrix4x4 temp;
	m_light_rot_matrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(-0.707f);
	m_light_rot_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(0.707f);
	m_light_rot_matrix *= temp;

	m_light_rot_matrix.setRotationY(0.707);//set the Roation in the Tranform matrix

}

void SpaceShooterGame::updateSpaceShip()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	m_spaceship_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_spaceship_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;



	if (m_spaceship_rot.m_x >= 1.57f)
		m_spaceship_rot.m_x = 1.57f;

	else if (m_spaceship_rot.m_x <= -1.57f)
		m_spaceship_rot.m_x = -1.57f;
	temp.setIdentity();
	temp.setRotationX(m_spaceship_rot.m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_spaceship_rot.m_y);
	world_cam *= temp;

	m_spaceship_pos  = m_spaceship_pos  + world_cam.getZDirection() * (m_forward) *m_spaceship_speed * m_delta_time;

	
}

void SpaceShooterGame::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{
	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());
	for (size_t m = 0; m < mesh->getNumMaterialSlots(); m++)
	{
		if (m >= list_materials.size()) break;
		MaterialSlot mat = mesh->getMaterialSlot(m);
		GraphicsEngine::get()->setMaterial(list_materials[m]);
		// FINALLY DRAW THE TRIANGLE
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}
}


SpaceShooterGame::~SpaceShooterGame()
{
}

void SpaceShooterGame::onCreate()
{


	//#pragma region Loading Textures
	//	m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\wall.jpg");
	//	m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\brick.png");
	//	m_earth_color_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\earth_color.jpg");
	//#pragma endregion
	//#pragma region Loading Meshes
	//

	////m_sand_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\sand.jpg");

	////m_barrel_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\barrel.jpg");
	////m_brick_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\house_brick.jpg");
	////m_windows_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\house_windows.jpg");
	////m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\house_wood.jpg");
	//	//Mesh
	//	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile
	//	(L"Meshes\\scene.obj");
	//	m_torus_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile
	//	(L"Meshes\\torus.obj");
	//	m_suzanne_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile
	//	(L"Meshes\\suzanne.obj");
	//	m_plane_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile
	//	(L"Meshes\\plane.obj");
	//#pragma endregion

	////m_terrain_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\terrain.obj");
	////m_house_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\house.obj");

	Window::onCreate();

	InputSystem::get()->addListener(this);//Observer desinge pattern. This class can listen to INput Events

	m_play_state = true;
	InputSystem::get()->showCursor(false);

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\stars_map.jpg");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\sphere.obj");

	m_spaceship_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\spaceship.jpg");
	m_spaceship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\spaceship.obj");

	m_asteroid_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Textures\\asteroid.jpg");
	m_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Meshes\\asteroid.obj");

	////m_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	////m_mat.get()->addTexture(m_wall_tex);
	////m_mat.get()->setCullMode(CULL_MODE_BACK);//Set different culling mode. Now only the front of the mesh can be seen.

	//m_earth_mat = GraphicsEngine::get()->createMaterial(m_mat);
	//m_earth_mat.get()->addTexture(m_earth_color_tex);
	//m_earth_mat.get()->setCullMode(CULL_MODE_BACK);//Set different culling mode. Now only the front of the mesh can be seen.

	////m_terrain_mat = GraphicsEngine::get()->createMaterial(m_mat);
	////m_terrain_mat.get()->addTexture(m_sand_tex);
	////m_terrain_mat.get()->setCullMode(CULL_MODE_BACK);//Set different culling mode. Now only the front of the mesh can be seen.

	////m_barrel_mat = GraphicsEngine::get()->createMaterial(m_mat);
	////m_barrel_mat.get()->addTexture(m_barrel_tex);
	////m_barrel_mat.get()->setCullMode(CULL_MODE_BACK);//Set different culling mode. Now only the front of the mesh can be seen.

	////m_brick_mat = GraphicsEngine::get()->createMaterial(m_mat);
	////m_brick_mat.get()->addTexture(m_brick_tex);
	////m_brick_mat.get()->setCullMode(CULL_MODE_BACK);//Set different culling mode. Now only the front of the mesh can be seen.

	////m_windows_mat = GraphicsEngine::get()->createMaterial(m_mat);
	////m_windows_mat.get()->addTexture(m_windows_tex);
	////m_windows_mat.get()->setCullMode(CULL_MODE_BACK);//Set different culling mode. Now only the front of the mesh can be seen.

	////m_wood_mat = GraphicsEngine::get()->createMaterial(m_mat);
	////m_wood_mat.get()->addTexture(m_wood_tex);
	////m_wood_mat.get()->setCullMode(CULL_MODE_BACK);//Set different culling mode. Now only the front of the mesh can be seen.
	m_base_mat = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	m_base_mat->setCullMode(CULL_MODE_BACK);

	m_asteroid_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_asteroid_mat->addTexture(m_asteroid_tex);
	m_asteroid_mat->setCullMode(CULL_MODE_BACK);

	m_spaceship_mat = GraphicsEngine::get()->createMaterial(m_base_mat);
	m_spaceship_mat->addTexture(m_spaceship_tex);
	m_spaceship_mat->setCullMode(CULL_MODE_BACK);


	m_sky_mat = GraphicsEngine::get()->createMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	m_sky_mat.get()->addTexture(m_sky_tex);
	m_sky_mat.get()->setCullMode(CULL_MODE_FRONT);//Set different culling mode. Now only the back of the mesh can be seen.

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	m_list_materials.reserve(32);
}
//Update every thing
void SpaceShooterGame::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();
	//COMPUTE TRANFORMATION MATRICES
	this->update();
	//RENDER
	this->render();
}
//Release reasources when done 
void SpaceShooterGame::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}
//When focus on the Window is gained
void SpaceShooterGame::onFocus()
{
	InputSystem::get()->addListener(this);
}
//When focus on the Window is lost.
void SpaceShooterGame::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void SpaceShooterGame::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);
	this->render();
}

void SpaceShooterGame::onKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rot_x += 3.14f*m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f*m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rot_y += 3.14f*m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_rightward = 1.0f;
	}
	else if (key == 'O')
	{
		m_light_radius -= 1.0f * m_delta_time;
	}
	else if (key == 'P')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_light_radius += 1.0f * m_delta_time;
	}

}

void SpaceShooterGame::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == VK_ESCAPE)
	{
		if(m_play_state)
		{
			m_play_state = false ;
			InputSystem::get()->showCursor(!m_play_state);
		}
	}

	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = this->getScreenSize();
		if (m_fullscreen_state)
		{
			m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
		}
	}
}

void SpaceShooterGame::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	RECT win_size = this->getClientWindowRect();

	int width = (win_size.right - win_size.left);
	int height = (win_size.bottom - win_size.top);

	m_delta_mouse_x = (int)(mouse_pos.m_x - (win_size.left + (width / 2.0f))) ;
	m_delta_mouse_y = (int)(mouse_pos.m_y - (win_size.top + (height / 2.0f)));


	//Changing rotation offset based on the X and Y position of the mouse

	InputSystem::get()->SetCursorPosition(Point(win_size.left + (int)(width / 2.0f), win_size .top+ (int)(height / 2.0f)));
}

void SpaceShooterGame::onLeftMouseDown(const Point& mouse_pos)
{

}

void SpaceShooterGame::onLeftMouseUp(const Point& mouse_pos)
{

}

void SpaceShooterGame::onRightMouseDown(const Point& mouse_pos)
{

		if (!m_play_state)
		{
			m_play_state = true;
			InputSystem::get()->showCursor(!m_play_state);
		}
}

void SpaceShooterGame::onRightMouseUp(const Point& mouse_pos)
{
}