#include "AppWindow.h"


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
	RECT rc;
	rc = getClientWindowRect();
	//initializing the swap chain and setting it width and height
	m_swap_chain->init(this->m_hwnd, rc.right- rc.left, rc.bottom - rc.top);
	


}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->ClearRenderTargetColor
	(this->m_swap_chain, 1, 1, 1, 1);




	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	GraphicsEngine::get()->release();

}
