//This is Very import file. This Contans most of the Functions and Variable types need the Set up a Window Application
#include <DX3D/Window/Window.h>
#include <Windows.h>
#include <exception>
//this   function   is     used   for	   handling    Envents
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//The WndProc a function that processes messages sent to a window and determines how the application responds to different events using these messages.
	switch (msg)
	{

		///Event For When the Window Is Created
	case WM_CREATE:
	{
		/// Event fired when the window is created
		/// collected here..

		break;//End of Case
	}
	case WM_SIZE: 
	{
		//Event fired each time the window is resized.
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onSize(window->getClientSize());
		break;
	}

	case WM_SETFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		//Fire Event When focus on this window is Gained
		if(window)	window->onFocus();

		break;
	}
	case WM_KILLFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		//Fire Event When focus on this window is lost
		window->onKillFocus();
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int wheelDelta = GET_WHEEL_DELTA_WPARAM(wparam);
		int linesToScroll = wheelDelta / WHEEL_DELTA;
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		break;
	}
	///Event For When the Winfow is Closed/Destroyed
	case WM_DESTROY:
	{
		//This Helps Retrive the Window becuase the variable is stored in another scope, so in order to get the Window by refference this Function must be called  
		Window* window = (Window*)GetWindowLongPtr ( hwnd, GWLP_USERDATA );
		///Destroys/Closes the Window
		window->onDestroy ( );
		///Makes the Meessaging System Stop 
		////::PostQuitMessage(0);
		break;//End of Case
	}
	case WM_CLOSE:
	{
		::PostQuitMessage ( 0 );
		
		break;
	}
	///If none Of the Events are meet or Some Other Event is meet Which is not Listed; For Example WM_MOVING whcih Triggers a Even when the USer moves the WIndow then the swicth will default.
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);///Returning the data Back to let the Processor know that the Events Weren't Met;

	}

	return NULL;///Returning Number zero, Letting the the Computer Know that the Events in the Swicth were met;
}
//This Funtion is Called For Initializing/Setting up the Window.
Window::Window()
{
	///Setting up WNDCLASSEX object
	WNDCLASSEX wc = {};//Creating a Wondow Variable 
	wc.cbSize = sizeof(WNDCLASSEX);//Size of the Window Class
	wc.lpszClassName = L"MyWindowClass";///The Name of the Application, THis Text is Shown on the top of the Window;
	wc.lpfnWndProc = &WndProc;//Referencing the WndProc the a Function pointer, which will constantly call WndProc, allowing the the WndProc the check for events
	///If the registration of WIndow class fail, the function will return false;
	auto classId = ::RegisterClassEx ( &wc );
	if (!classId) DX3DError(L"Window not created successfully");

	RECT rc = { 0,0, m_size.width, m_size.height};
	AdjustWindowRect ( &rc, WS_EX_OVERLAPPEDWINDOW, false );

	///Creates a new window
	m_hwnd = ::CreateWindowEx
	(
		NULL, MAKEINTATOM(classId), 
		L"Joshua | DirectX 3D Game", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, NULL, this
	);

	///if the creation fail return false
	if (!m_hwnd) DX3DError("Failed to create window");

	auto hwnd = static_cast<HWND>(m_hwnd);

	SetWindowLongPtr ( hwnd, GWLP_USERDATA, (LONG_PTR)this );

	///Shows the Window Using the SW_SHOW Command in the form or a Macro
	ShowWindow( hwnd, SW_SHOW);
	///This WIll Update the Window, ONe Reason WHy this Function is Needed is becuase it will help call the WndProc which checks for Events
	UpdateWindow( hwnd );
}





////This Function will do a Broadcast to check for messages.
//bool Window::broadcast()
//{
//	///Declarring/Creating a Varible Whcih Will hold the Messages from the Window
//	MSG msg;//MSG is short for message.
//		
//	if(!this->m_is_init)
//	{
//		/// .. and then stored for later lookup
//		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
//		this->onCreate();
//		this->m_is_init = true;
//	}
//
//	
//	///A message loop that continuously retrieves messages from the application's message queue
//	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
//	{
//		///Translates the Message for the next function to be able to read the Message
//		TranslateMessage(&msg);
//		//Runs some even lower level code in the backround to Help get the Message in the Parameters of the WnProc Function
//		//Based on the Message this Could be used the trgger certian Window Events in the WnProc's Switch cases.
//		DispatchMessage(&msg);
//	}
//
//	this->onUpdate();
//	///Gives the Computer a 1 Milli-second to realeve some streess on the CPU a bit before conutinuing
//	Sleep(1);
//
//	return true;
//}
//
//
//
////Return 1/True if the window is Running; If not it return 0/false 
//bool Window::isRun()
//{
//
//	if (m_is_run) broadcast();
//	return m_is_run;
//}

Rect Window::getClientSize ()
{
	RECT rc = {};
	auto hwnd = static_cast<HWND>(m_hwnd);
	::GetClientRect( hwnd, /*ref OutPut*/&rc);
	::ClientToScreen( hwnd, (LPPOINT)& rc.left);
	::ClientToScreen( hwnd, (LPPOINT)& rc.right);
	return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };//Return RECT rc variable

}
Rect Window::getScreenSize()
{
	RECT rc = {};
	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	return { 0, 0, rc.right - rc.left, rc.bottom - rc.top };
}
//higher level Code Gose in Here for when the Window is Created/Initialized 
void Window::onCreate()
{
}
//higher level Code Gose in Here for when the Window Updates
void Window::onUpdate()
{
}
//higher level Code Gose in Here for when the Window is Closed/Destroyed
//Sets the m_is_run bool variable to false, showing  that the window is not up and running
void Window::onDestroy()
{

}

void Window::onFocus()
{

}

void Window::onKillFocus()
{

}

void Window::onSize( const Rect& size )
{

}

//Called to delete resoures when not needed
Window::~Window()
{
	//Destroy the window
	DestroyWindow( static_cast<HWND>(m_hwnd) );
}