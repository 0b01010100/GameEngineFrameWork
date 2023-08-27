#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

//The class for the App Window
class  __single_inheritance AppWindow : public Window 
{
public:
	AppWindow();
	~AppWindow();

/*These Events Area Virtual so another class can inherit and Impliment to functions in it own way.*/
	//This Event is Called for when the Window is Created
	virtual void onCreate() override;
	//Event called for when every frame if the Window is not closed
	virtual void onUpdate() override;
	//Event Called for when the Window is Closed
	virtual void onDestroy() override;
private:
	//A variable that helps manage the Monitor of a Screen For Prensetation on the Screen.
	SwapChain* m_swap_chain = nullptr;
};

