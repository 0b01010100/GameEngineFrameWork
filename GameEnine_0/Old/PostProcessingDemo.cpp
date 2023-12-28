#include "PostProcessingDemo.h"
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
	Matrix4x4 m_world;//represents the transformation of an object in world space
	Matrix4x4 m_view;//camera's view transformation
	Matrix4x4 m_proj;//projection matrix used for perspective projection
	Vector4D m_light_direction;//Direction of the light normalised 
	Vector4D m_camera_position;//the camera's position
	Vector4D m_light_position = { 0, 1, 0, 0 };//The position of the point light
	float m_light_radius = 4.0f;//Radius of the point light
	float m_time = 0.0f;// delta time - time elasped from frame 1 to 2.
};//cbuffer
__declspec(align(16))
struct DistortionEffectData
{
	float m_distortion_level = 0.5f;// delta time - time elasped from frame 1 to 2.
};//cbuffer

PostProcessingDemo::PostProcessingDemo ( )
{
}

void PostProcessingDemo::render ( )
{
//Scene Rendered to Render Target
//---------------------------------------------
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->clearRenderTargetColor ( this->m_render_target,
		0, 0.3f, 0.4f, 1 );
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->clearDepthStencil ( this->m_depth_stencil );
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->setRenderTarget ( m_render_target, m_depth_stencil );


	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	Rect view_size = m_render_target->getSize ( );
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->setViewportSize ( view_size.width, view_size.height );

	//RENDER SPACESHIP
	m_list_materials.clear ( );
	m_list_materials.push_back ( m_spaceship_mat );
	updateModel ( m_current_spaceship_pos, m_current_spaceship_rot, Vector3D ( 1, 1, 1 ), m_list_materials );
	drawMesh ( m_spaceship_mesh, m_list_materials );

	//RENDER ASTEROID
	m_list_materials.clear ( );
	m_list_materials.push_back ( m_asteroid_mat );
	for (unsigned int i = 0; i < 200; i++)
	{
		updateModel ( m_asterids_pos[i], m_asterids_rot[i], m_asterids_scale[i], m_list_materials );
		drawMesh ( m_asteroid_mesh, m_list_materials );
	}


	//RENDER SKYBOX/SPHERE
	m_list_materials.clear ( );
	m_list_materials.push_back ( m_sky_mat );
	drawMesh ( m_sky_mesh, m_list_materials );

	//------------------------------------------------------
	//------------------------------------------------------



	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->clearRenderTargetColor ( this->m_swap_chain,
		0, 0.3f, 0.4f, 1 );
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect ( );
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->setViewportSize ( rc.right - rc.left, rc.bottom - rc.top );

	DistortionEffectData effect_data;
	effect_data.m_distortion_level = m_distortion_level;

	m_list_materials.clear ( );
	m_list_materials.push_back ( m_post_process_mat );
	m_post_process_mat->setData ( &effect_data, sizeof ( DistortionEffectData ) );
	drawMesh ( m_quad_mesh, m_list_materials );

	//DONE coloring on the render target now 
	//present it so it can be shown from the human eye.
	m_swap_chain->present ( true );


	m_old_delta = m_new_delta;//Will soon be the time elapsed from the old frame to a new frame
	m_new_delta = ::GetTickCount64 ( );//Set the m_new_delta variable to current tick count

	// If m_old_delta is non-zero, this part calculates the time difference between the current frame 
	//m_new_delta and the previous frame m_old_delta. The difference is divided by 1000.0f, to convert it into seconds 
	//m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_delta_time = 1.0f / 60.0f;
	m_time += m_delta_time;//updating the program's notion of the current 
}

void PostProcessingDemo::update ( )//Update
{
	this->updateSpaceShip ( );
	this->updateThirdPersonCamera ( );
	this->updateLight ( );
	this->updateSkyBox ( );

}

void PostProcessingDemo::updateModel ( Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials )
{
	constant cc = {};

	Matrix4x4 temp = {};
	cc.m_world.setIdentity ( );


	temp.setIdentity ( );
	temp.setScale ( scale );
	cc.m_world *= temp;

	temp.setIdentity ( );
	temp.setRotationX ( rotation.m_x );
	cc.m_world *= temp;

	temp.setIdentity ( );
	temp.setRotationY ( rotation.m_y );
	cc.m_world *= temp;

	temp.setIdentity ( );
	temp.setRotationZ ( rotation.m_z );
	cc.m_world *= temp;


	temp.setIdentity ( );
	temp.setTranslation ( position );
	cc.m_world *= temp;

	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation ( );

	cc.m_light_position = m_light_position;
	cc.m_light_radius = m_light_radius;//Set the radius varaible equal to the radius varaible in the class 
	cc.m_light_direction = m_light_rot_matrix.getZDirection ( );
	cc.m_time = m_time;
	for (size_t m = 0; m < list_materials.size ( ); m++)
	{
		list_materials[m]->setData ( &cc, sizeof ( constant ) );//Update changed data in to gpu memory to be used in the shader
	}
}

void PostProcessingDemo::updateCamera ( )
{

	Matrix4x4 world_cam, temp;
	world_cam.setIdentity ( );

	temp.setIdentity ( );
	temp.setRotationX ( m_cam_rot.m_x );
	world_cam *= temp;

	temp.setIdentity ( );
	temp.setRotationY ( m_cam_rot.m_y );
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation ( ) + world_cam.getZDirection ( ) * (m_forward * 0.05f);

	new_pos = new_pos + world_cam.getXDirection ( ) * (m_rightward * 0.05f);

	world_cam.setTranslation ( new_pos );

	m_world_cam = world_cam;

	world_cam.inverse ( );

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect ( ).right - this->getClientWindowRect ( ).left);
	int height = (this->getClientWindowRect ( ).bottom - this->getClientWindowRect ( ).top);


	this->m_proj_cam.setPerspectiveFovLH ( 1.57f, ((float)width / (float)height), 0.1f, 600.0f );

}

void PostProcessingDemo::updateThirdPersonCamera ( )
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity ( );

	m_cam_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_cam_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;


	if (m_cam_rot.m_x >= 1.57f)
		m_cam_rot.m_x = 1.57f;

	else if (m_cam_rot.m_x <= -1.57f)
		m_cam_rot.m_x = -1.57f;
	m_current_cam_rot = Vector3D::lerp ( m_current_cam_rot, m_cam_rot, 3.0f * m_delta_time );
	temp.setIdentity ( );
	temp.setRotationX ( m_current_cam_rot.m_x );
	world_cam *= temp;

	temp.setIdentity ( );
	temp.setRotationY ( m_current_cam_rot.m_y );
	world_cam *= temp;

	if (m_forward)
	{
		if (m_turbo_mode)
		{
			if (m_forward > 0.0f) m_cam_distance = 19.0f;
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

	m_current_cam_distance = lerp ( m_current_cam_distance, m_cam_distance, 2.0f * m_delta_time );

	m_cam_pos = m_current_spaceship_pos;

	Vector3D new_pos = m_cam_pos/*Retrive the Pos of the Camera*/ + world_cam.getZDirection ( ) * (-m_current_cam_distance);//Offset Cam's relative Y by times -m_current_cam_distance.
	new_pos = new_pos + world_cam.getYDirection ( ) * (5.0f);//Offset Cam's relative Y by times 5
	world_cam.setTranslation ( new_pos );

	m_world_cam = world_cam;

	world_cam.inverse ( );

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect ( ).right - this->getClientWindowRect ( ).left);
	int height = (this->getClientWindowRect ( ).bottom - this->getClientWindowRect ( ).top);


	this->m_proj_cam.setPerspectiveFovLH ( 1.57f, ((float)width / (float)height), 0.1f, 5000.0f );
}

void PostProcessingDemo::updateSkyBox ( )
{
	constant cc;

	cc.m_world.setIdentity ( );
	cc.m_world.setScale ( Vector3D ( 4000.0f, 4000.0f, 4000.0f ) );
	cc.m_world.setTranslation ( m_world_cam.getTranslation ( ) );
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_mat->setData ( &cc, sizeof ( constant ) );
}

void PostProcessingDemo::updateLight ( )
{
	Matrix4x4 temp;
	m_light_rot_matrix.setIdentity ( );

	temp.setIdentity ( );
	temp.setRotationX ( -0.707f );
	m_light_rot_matrix *= temp;

	temp.setIdentity ( );
	temp.setRotationY ( 0.707f );
	m_light_rot_matrix *= temp;

	m_light_rot_matrix.setRotationY ( 0.707 );//set the Roation in the Tranform matrix

}

void PostProcessingDemo::updateSpaceShip ( )
{
	Matrix4x4 world_model, temp;
	world_model.setIdentity ( );

	m_spaceship_rot.m_x += m_delta_mouse_y * m_delta_time * 0.1f;
	m_spaceship_rot.m_y += m_delta_mouse_x * m_delta_time * 0.1f;



	if (m_spaceship_rot.m_x >= 1.57f)
		m_spaceship_rot.m_x = 1.57f;

	else if (m_spaceship_rot.m_x <= -1.57f)
		m_spaceship_rot.m_x = -1.57f;

	m_current_spaceship_rot = Vector3D::lerp ( m_current_spaceship_rot, m_spaceship_rot, 5.0f * m_delta_time );
	temp.setIdentity ( );
	temp.setRotationX ( m_current_spaceship_rot.m_x );
	world_model *= temp;

	temp.setIdentity ( );
	temp.setRotationY ( m_current_spaceship_rot.m_y );
	world_model *= temp;


	m_spaceship_speed = 125.f;
	if (m_turbo_mode)
	{
		m_spaceship_speed = 305.0f;

		if (m_forward != 0.0f) 
		{
			m_distortion_level -= m_delta_time * 0.4f;
			if (m_distortion_level <= 0.6f) m_distortion_level = 0.6;
		}
	}
	else
	{
		m_distortion_level += m_delta_time * 0.4f;
		if (m_distortion_level >= 1.0f) m_distortion_level = 1.0f;
	}
	m_spaceship_pos = m_spaceship_pos + world_model.getZDirection ( ) * (m_forward)*m_spaceship_speed * m_delta_time;

	m_current_spaceship_pos = Vector3D::lerp ( m_current_spaceship_pos, m_spaceship_pos, 3.0f * m_delta_time );
}

void PostProcessingDemo::drawMesh ( const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials )
{
	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->setVertexBuffer ( mesh->getVertexBuffer ( ) );
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->setIndexBuffer ( mesh->getIndexBuffer ( ) );
	for (size_t m = 0; m < mesh->getNumMaterialSlots ( ); m++)
	{
		if (m >= list_materials.size ( )) break;
		MaterialSlot mat = mesh->getMaterialSlot ( m );
		GraphicsEngine::get ( )->setMaterial ( list_materials[m] );
		// FINALLY DRAW THE TRIANGLE
		GraphicsEngine::get ( )->getRenderSystem ( )->getImmediateDeviceContext ( )->drawIndexedTriangleList ( mat.num_indices, 0, mat.start_index );
	}
}


PostProcessingDemo::~PostProcessingDemo ( )
{
}

void PostProcessingDemo::onCreate ( )
{

	Window::onCreate ( );

	InputSystem::get ( )->addListener ( this );//Observer desinge pattern. This class can listen to INput Events

	m_play_state = true;
	InputSystem::get ( )->showCursor ( false );

	RECT rc = this->getClientWindowRect ( );
	m_swap_chain = GraphicsEngine::get ( )->getRenderSystem ( )->createSwapChain ( this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top );

	srand ( (unsigned int)time ( NULL ) );

	for (unsigned int i = 0; i < 200; i++)
	{
		m_asterids_pos[i] = Vector3D ( rand ( ) % 4000 + (-2000), rand ( ) % 4000 + (-2000), rand ( ) % 4000 + (-2000) );
		m_asterids_rot[i] = Vector3D ( rand ( ) % 628 / 100, rand ( ) % 628 / 100, rand ( ) % 628 / 100 );
		float scale = rand ( ) % 20 + (6);
		m_asterids_scale[i] = Vector3D ( scale, scale, scale );
	}

	m_sky_tex = GraphicsEngine::get ( )->getTextureManager ( )->createTextureFromFile ( L"Textures\\stars_map.jpg" );
	m_sky_mesh = GraphicsEngine::get ( )->getMeshManager ( )->createMeshFromFile ( L"Meshes\\sphere.obj" );

	m_spaceship_tex = GraphicsEngine::get ( )->getTextureManager ( )->createTextureFromFile ( L"Textures\\spaceship.jpg" );
	m_spaceship_mesh = GraphicsEngine::get ( )->getMeshManager ( )->createMeshFromFile ( L"Meshes\\spaceship.obj" );

	m_asteroid_tex = GraphicsEngine::get ( )->getTextureManager ( )->createTextureFromFile ( L"Textures\\asteroid.jpg" );
	m_asteroid_mesh = GraphicsEngine::get ( )->getMeshManager ( )->createMeshFromFile ( L"Meshes\\asteroid.obj" );


	m_base_mat = GraphicsEngine::get ( )->createMaterial ( L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl" );
	m_base_mat->setCullMode ( CULL_MODE_BACK );

	m_asteroid_mat = GraphicsEngine::get ( )->createMaterial ( m_base_mat );
	m_asteroid_mat->addTexture ( m_asteroid_tex );
	m_asteroid_mat->setCullMode ( CULL_MODE_BACK );

	m_spaceship_mat = GraphicsEngine::get ( )->createMaterial ( m_base_mat );
	m_spaceship_mat->addTexture ( m_spaceship_tex );
	m_spaceship_mat->setCullMode ( CULL_MODE_BACK );


	m_sky_mat = GraphicsEngine::get ( )->createMaterial ( L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl" );
	m_sky_mat.get ( )->addTexture ( m_sky_tex );
	m_sky_mat.get ( )->setCullMode ( CULL_MODE_FRONT );//Set different culling mode. Now only the back of the mesh can be seen.

	m_post_process_mat = GraphicsEngine::get ( )->createMaterial ( L"PostProcessVS.hlsl", L"DistortionEffect.hlsl" );
	m_post_process_mat.get ( )->setCullMode ( CULL_MODE_BACK );//Set different culling mode. Now only the back of the mesh can be seen.




	m_world_cam.setTranslation ( Vector3D ( 0, 0, -2 ) );

	VertexMesh quad_vertex_list[] = {
		VertexMesh(Vector3D( -1, -1, 0 ), Vector2D(0,1 ), Vector3D(), Vector3D(), Vector3D()),
		VertexMesh(Vector3D( -1, 1, 0 ), Vector2D(0,0 ), Vector3D(), Vector3D(), Vector3D()),
		VertexMesh(Vector3D( 1, 1, 0 ), Vector2D(1,0 ), Vector3D(), Vector3D(), Vector3D()),
		VertexMesh(Vector3D( 1, -1, 0 ), Vector2D(1,1 ), Vector3D(), Vector3D(), Vector3D()),
	};

	unsigned int quad_index_list[6] =
	{
			0, 1, 2,
			2, 3, 0
	};

	MaterialSlot quad_mat_slots[] = 
	{
		{0, 6, 0}
	};
	m_quad_mesh = GraphicsEngine::get ( )->getMeshManager ( )->createMesh (
		quad_vertex_list, 4,
		quad_index_list, 6, 
		quad_mat_slots, 1 );


	m_list_materials.reserve ( 32 );

	m_render_target = GraphicsEngine::get ( )->getTextureManager ( )->createTexture ( Rect ( rc.right - rc.left, rc.bottom - rc.top ), Texture::Type::RenderTarget );
	m_depth_stencil = GraphicsEngine::get ( )->getTextureManager ( )->createTexture ( Rect ( rc.right - rc.left, rc.bottom - rc.top ), Texture::Type::DepthStencil );

	m_post_process_mat->addTexture ( m_render_target );
}
//Update every thing
void PostProcessingDemo::onUpdate ( )
{
	Window::onUpdate ( );

	InputSystem::get ( )->update ( );
	//COMPUTE TRANFORMATION MATRICES
	this->update ( );
	//RENDER
	this->render ( );

	m_delta_mouse_x = 0;
	m_delta_mouse_y = 0;
}
//Release reasources when done 
void PostProcessingDemo::onDestroy ( )
{
	Window::onDestroy ( );
	m_swap_chain->setFullScreen ( false, 1, 1 );
}
//When focus on the Window is gained
void PostProcessingDemo::onFocus ( )
{
	InputSystem::get ( )->addListener ( this );
}
//When focus on the Window is lost.
void PostProcessingDemo::onKillFocus ( )
{
	InputSystem::get ( )->removeListener ( this );
}

void PostProcessingDemo::onSize ( )
{
	RECT rc = this->getClientWindowRect ( );
	m_swap_chain->resize ( rc.right - rc.left, rc.bottom - rc.top );
	//Resize the render target view anddepth stencil
	m_render_target = GraphicsEngine::get ( )->getTextureManager ( )->createTexture ( Rect ( rc.right - rc.left, rc.bottom - rc.top ), Texture::Type::RenderTarget );
	m_depth_stencil = GraphicsEngine::get ( )->getTextureManager ( )->createTexture ( Rect ( rc.right - rc.left, rc.bottom - rc.top ), Texture::Type::DepthStencil );

	m_post_process_mat->removeTexture ( 0 );//Remove old render target texture
	m_post_process_mat->addTexture ( m_render_target );// add a new one

	this->update ( );
	this->render ( );
}

void PostProcessingDemo::onKeyDown ( int key )
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
	else if (key == VK_SHIFT)
	{
		m_turbo_mode = true;
	}
}

void PostProcessingDemo::onKeyUp ( int key )
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == VK_ESCAPE)
	{
		if (m_play_state)
		{
			m_play_state = false;
			InputSystem::get ( )->showCursor ( !m_play_state );
		}
	}

	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = this->getScreenSize ( );
		if (m_fullscreen_state)
		{
			m_swap_chain->setFullScreen ( m_fullscreen_state, size_screen.right, size_screen.bottom );
		}
	}
	else if (key == VK_SHIFT)
	{
		m_turbo_mode = false;
	}
}

void PostProcessingDemo::onMouseMove ( const Point& mouse_pos )
{
	if (!m_play_state) return;

	RECT win_size = this->getClientWindowRect ( );

	int width = (win_size.right - win_size.left);
	int height = (win_size.bottom - win_size.top);

	m_delta_mouse_x = (int)(mouse_pos.m_x - (int)(win_size.left + (width / 2.0f)));
	m_delta_mouse_y = (int)(mouse_pos.m_y - (int)(win_size.top + (height / 2.0f)));


	//Changing rotation offset based on the X and Y position of the mouse

	InputSystem::get ( )->SetCursorPosition ( Point ( win_size.left + (int)(width / 2.0f), win_size.top + (int)(height / 2.0f) ) );
}

void PostProcessingDemo::onLeftMouseDown ( const Point& mouse_pos )
{

}

void PostProcessingDemo::onLeftMouseUp ( const Point& mouse_pos )
{

}

void PostProcessingDemo::onRightMouseDown ( const Point& mouse_pos )
{

	if (!m_play_state)
	{
		m_play_state = true;
		InputSystem::get ( )->showCursor ( !m_play_state );
	}
}

void PostProcessingDemo::onRightMouseUp ( const Point& mouse_pos )
{
}