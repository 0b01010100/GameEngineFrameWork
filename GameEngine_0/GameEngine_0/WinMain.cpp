#include "AppWindow.h"
int WinMain(HINSTANCE hInstance, HINSTANCE InstancePrv, LPSTR,int Cmd)
{
	AppWindow app;
	if (app.init())
	{
		while (app.isRun())
		{
			app.broadcast();
		}
	}
	
	return 0;
}