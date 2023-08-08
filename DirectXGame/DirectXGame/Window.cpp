#include "Window.h"

Window* window = nullptr;

Window::Window()
{
	
}
LRESULT CALLBACK WindProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		// Event fired when the window is created
		window->OnCreate();
		break;
	}
	case WM_DESTROY:
	{
		// Event fired when the window is destroyed
		window->OnDestroy();
		::PostQuitMessage(0);
		break;
	}
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;



}
bool Window::init()
{
	WNDCLASSEX wc;
	if (!window)
		window = this;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
    wc.style = NULL;

	wc.lpfnWndProc = &WindProc;


	if(!::RegisterClassEx(&wc)) // If the registration of class will fail, the function will return false
	{
		return false;
	}

	//Creation of the Window
	m_hwnd=::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,1024, 768, NULL, NULL, NULL, NULL);
	
	//If the creation fail return false
	if (!m_hwnd)
		return false;

	//Show up the Window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	//Set this flag to indecate that the WIndow in initalized and running
	m_is_run = true;


	return true;
}

bool Window::BroadCast()
{
	MSG msg;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	window->OnUpdate();

	Sleep(0);

	return true;
}

bool Window::release()
{
	if(!::DestroyWindow(m_hwnd))
	{
		return false;
	}
	return true;
}

bool Window::isRun()
{

	return m_is_run;
}

void Window::OnDestroy()
{
	m_is_run = false;
}

Window::~Window()
{
	
}