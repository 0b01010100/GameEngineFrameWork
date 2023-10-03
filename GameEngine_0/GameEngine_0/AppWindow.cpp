#include "AppWindow.h"
#include "Vector3.h"
#include "Matrix4x4.h"

__declspec(align(16))
class constant
{
public:
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;

	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::updateQuadPos()
{
	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount64() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount64();

	m_angle += 1.57f * m_delta_time;
	constant cc;
	//cc.m_time = m_angle;

	cc.m_time = ::GetTickCount64();
	float d = std::lerp(0.f, 1.f,( sin(m_angle) + 1.0f) / 2);
	cc.m_world.setTrasnlation(Vector3D(-d, d, 0));
	cc.m_view.setIdentity();
	cc.m_proj.setOrthLH(
		(this->getClientWindowRect().right - this->getClientWindowRect().left/400.0f),
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top/400.0f),
		-1.0f,
		1.0f
	);
	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	struct vertex list[] 
	{
		//X - Y - Z
		{Vector3D(-0.5f,-0.5f,0.0f),    Vector3D(-0.32f,-0.11f,0.0f),   Vector3D(0,0,0),  Vector3D(0,1,0)}, // POS1
		{Vector3D(-0.5f,0.5f,0.0f),     Vector3D(-0.11f,0.78f,0.0f),    Vector3D(1,1,0),  Vector3D(0,1,1)}, // POS2
		{Vector3D(0.5f,-0.5f,0.0f),     Vector3D(0.75f,-0.73f,0.0f),  Vector3D(0,0,1),  Vector3D(1,0,0) },// POS2
		{Vector3D(0.5f,0.5f,0.0f),      Vector3D(0.88f,0.77f,0.0f),    Vector3D(1,1,1),  Vector3D(0,0,1) },

		{Vector3D(-0.5f,-0.5f,0.0f),    Vector3D(-0.5f,-0.11f,0.0f),   Vector3D(0,22,0),  Vector3D(0,1,0)}, // POS1
		{Vector3D(-0.5f,0.5f,0.0f),     Vector3D(-0.21f,0.78f,0.0f),    Vector3D(1,1,220),  Vector3D(0,1,1)}, // POS2
		{Vector3D(0.5f,-0.5f,0.0f),     Vector3D(1.f,-0.73f,0.0f),   Vector3D(0,0,1),  Vector3D(1,0,0) },// POS2
		{Vector3D(0.5f,0.5f,0.0f),      Vector3D(0.2f,0.77f,0.0f),    Vector3D(1,1,1),  Vector3D(0,0,1) }
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(struct vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;
	
	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	updateQuadPos();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);
	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}