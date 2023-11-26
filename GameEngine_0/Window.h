#pragma once
#include "Windows.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
class Window
{

public:
	//Initialize the window
	Window();

	bool isRun();

	RECT getClientWindowRect();
	RECT getScreenSize();
	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();


	//Release the window
	~Window();
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};