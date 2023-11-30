#pragma once

#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"
class SpaceShooterGame : public Window, public InputListener
{
	public:
		SpaceShooterGame();
		~SpaceShooterGame();

		// Inherited via Window
		virtual void onCreate() override;
		virtual void onUpdate() override;
		virtual void onDestroy() override;
		virtual void onFocus() override;
		virtual void onKillFocus() override;
		virtual void onSize() override;
		// Inherited via InputListener
		virtual void onKeyDown(int key) override;
		virtual void onKeyUp(int key) override;
		virtual void onMouseMove(const Point& mouse_pos) override;

		virtual void onLeftMouseDown(const Point& mouse_pos) override;
		virtual void onLeftMouseUp(const Point& mouse_pos) override;
		virtual void onRightMouseDown(const Point& mouse_pos) override;
		virtual void onRightMouseUp(const Point& mouse_pos) override;
	public:
		void render();
		void update();
		void updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials);
		void updateCamera();
		void updateThirdPersonCamera();
		void updateSkyBox();
		void updateLight();
		void updateSpaceShip();
		void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials);
	private:

		//VertexBufferPtr m_vb;
		//VertexShaderPtr m_vs;
		//PixelShaderPtr m_ps;
		//PixelShaderPtr m_sky_ps;
		//ConstantBufferPtr m_cb;
	/*	ConstantBufferPtr m_sky_cb;
		IndexBufferPtr m_ib;*/
		/*TexturePtr m_wall_tex;
		TexturePtr m_bricks_tex;
		TexturePtr m_earth_color_tex;
		TexturePtr m_earth_spec_tex;
		TexturePtr m_earth_night_tex;
		TexturePtr m_clouds_tex;
		TexturePtr m_sand_tex;*/


		//TexturePtr m_barrel_tex;
		//TexturePtr m_brick_tex;
		//TexturePtr m_windows_tex;
		//TexturePtr m_wood_tex;


		//MeshPtr m_mesh;
		//MeshPtr m_torus_mesh;
		//MeshPtr m_suzanne_mesh;
		//MeshPtr m_plane_mesh;

		//MeshPtr m_terrain_mesh;
		//MeshPtr m_house_mesh;



		//MaterialPtr m_mat;
		//MaterialPtr m_bricks_mat;
		//MaterialPtr m_earth_mat;

		//MaterialPtr m_terrain_mat;

		//MaterialPtr m_barrel_mat;
		//MaterialPtr m_brick_mat;
		//MaterialPtr m_windows_mat;
		//MaterialPtr m_wood_mat;
private:
	



	SwapChainPtr m_swap_chain;

	TexturePtr m_spaceship_tex;
	MeshPtr m_spaceship_mesh;
	MaterialPtr m_spaceship_mat;
	
	TexturePtr m_asteroid_tex;
	MeshPtr m_asteroid_mesh;
	MaterialPtr m_asteroid_mat;

	TexturePtr m_sky_tex;
	MeshPtr m_sky_mesh;
	MaterialPtr m_sky_mat;

	MaterialPtr m_base_mat;
private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_mouse_x;
	float m_delta_mouse_y;
	float m_cam_distance = 14.0f;

	float m_spaceship_speed = 125.0f;
	Vector3D m_cam_rot;
	Vector3D m_cam_pos;


	Vector3D m_spaceship_pos;
	Vector3D m_spaceship_rot;



	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;



	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;


	float m_time = 0.0f;
	float m_light_radius = 500;
	Matrix4x4 m_light_rot_matrix = {};
	bool m_play_state = false;
	bool m_fullscreen_state = false;

	std::vector <MaterialPtr> m_list_materials;

	Vector4D m_light_position;
};

