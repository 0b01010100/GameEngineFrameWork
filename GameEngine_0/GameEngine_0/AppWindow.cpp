#include "AppWindow.h"
#include "VecLib.c"//A local Libray with the Vertex and Vectex struct.

AppWindow::AppWindow()
{
}


AppWindow::~AppWindow()
{
} 

void AppWindow::onCreate()
{
	//An event that is called when the class is Created
	Window::onCreate();
	//This will Initilize the GraphicsEngine class, which is a class that helps manage how things are rendered on the screen.
	GraphicsEngine::get()->init();
	//Assinging the Swap Chain in the GraphicsEngine to the this classes Swap Chain Variable 
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	//initializing the swap chain and setting it width and height
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
	
	vertex list[] =
	{
		//X - Y - Z
		{ -0.5f,-0.5f,0.0f, 1.f, 0.f, 0.f}, // POS1
		{-0.5f,0.5f,0.0f,   0.f, 1.f, 0.f}, // POS2
		{ 0.5f,-0.5f,0.0f,   0.f, 0.f, 1.f},// POS2
		{ 0.5f,0.5f,0.0f,     1.f,  1.f, 0.f}
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	void* shader_byte_codee = nullptr;
	size_t size_shaderr = 0;
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_codee, &size_shaderr);

	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_codee, size_shaderr);
	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	//Clears to buffers, preparing them for rendering a new frame
	//Black color with 100% Opacity.
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor
	(this->m_swap_chain, 0, 2, 0, 1); 
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
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

