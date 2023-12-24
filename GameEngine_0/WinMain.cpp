#include "PostProcessingDemo.h"
#include "InputSystem.h"
int __stdcall WinMain
(
	HINSTANCE hInstance,
	HINSTANCE InstancePrv,
	LPSTR, 
	int nCmd
){
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (const std::exception&)
	{
		return -1;
	}

	{
		try
		{
			PostProcessingDemo app = {};
			while (app.isRun());
		}
		catch (const std::exception&)
		{
			InputSystem::release();
			GraphicsEngine::release();
			return -1 ;
		}

	}
	InputSystem::release();
	GraphicsEngine::release();
	return 0;
}