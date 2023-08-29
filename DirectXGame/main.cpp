#include "AppWindow.h"
//Programs entry point/ First Function to be called.
int main()//The Main Thread in the CPU calls this Functions.
{
	AppWindow app;//Declaring the app variable.
	if (app.init()) // calling the init to initlize the app class, while return a value which will help determine wheater to continue to the while.
	{
		while (app.isRun())//This loop Runs infinitely unless the Window Created is not up and running.
		{
			app.broadcast();//COnstantly checks to see if message were sent from the Window.
		}
	}
	return 0;
}