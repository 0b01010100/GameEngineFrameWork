#include "AppWindow.h"
int __stdcall WinMain
(
	HINSTANCE hInstance,
	HINSTANCE InstancePrv,
	LPSTR, 
	int nCmd
)
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