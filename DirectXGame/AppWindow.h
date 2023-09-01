#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	/*Inherited via Window*/ 
	//Called when the WIndow is first created
	virtual void onCreate() override;
	//Called when the window is up and running
	virtual void onUpdate() override;
	//Called when the Window s Destoryed
	virtual void onDestroy() override;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
};