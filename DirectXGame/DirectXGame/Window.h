#pragma once
#include <Windows.h>


class Window
{
public:
	Window();
	//Initialize the Window
	bool init();
	//Pickes the Event messages from the operating system
	bool BroadCast();
	//Realese the Window
	bool release();
	//A value that holds true or false based on wheather the Window in is Running or not.
	bool isRun();
	
	//Events Methods
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
    virtual void OnDestroy();
	~Window();
protected:
	HWND m_hwnd = NULL;
	bool m_is_run = false;
};

