#pragma once
#include <Windows.h>

//Custom Window Class 
class Window
{
public:
	Window();
	//Initialize the window
	bool init();
	//Broadcasts the Messages sent to the Window
	bool broadcast();
	//Release the window
	bool release();
	//Returns True if the Window is Up and Runnining and Returns false if the Window not Up and running
	bool isRun();
	//Gets the RECT of the Window Client
	RECT getClientWindowRect();
	//Set the Gloabl Window Handler equal to the the Local Window Handler 
	void setHWND(HWND hwnd);

	///EVENTS
	//High Level Code gose in here for when the Window is Created.
	virtual void onCreate();//
	//High Level code gose in here for when the window in Updated
	virtual void onUpdate();
	//High level code gose in here for when the window is Destroyed/Closed
	virtual void onDestroy();
	
	~Window();
protected:
	//Window Handel
	HWND m_hwnd;
	//Is set to either true or False for when the Window is Running or is Destroyed
	bool m_is_run;
};


