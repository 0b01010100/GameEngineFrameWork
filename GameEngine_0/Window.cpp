//This is Very import file. This Contans most of the Functions and Variable types need the Set up a Window Application
#include "Window.h"
#include "string"
#include <exception>
//这         函数    是      用      于      处理          事件
//this   function   is     used   for	   handling    Envents
//zhe   han shu   shi   yong   yu      chu           shijain
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
		if (window) window->onSize();
		break;
	}
	///Event For When the Winfow is Closed/Destroyed
	case WM_DESTROY:
	{
		//This Helps Retrive the Window becuase the variable is stored in another scope, so in order to get the Window by refference this Function must be called  
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		///Destroys/Closes the Window
		window->onDestroy();
		///Makes the Meessaging System Stop 
		::PostQuitMessage(0);
		break;//End of Case
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
	WNDCLASSEX wc;//Creating a Wondow Variable 
	///Below a Customizations of the Window WHich many be shown Visually on the Screen.
	wc.cbClsExtra = NULL;// This is Only Used if Extra Computer Memory should be used for the class, in bytes
	wc.cbSize = sizeof(WNDCLASSEX);//Size of the Window Class
	wc.cbWndExtra = NULL;//Extra memory to allocate for each window Instance of this Window class type, in bytes.
	wc.hbrBackground = NULL;///Color Of Window;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);///Type of Mouse Cursor;
	wc.hIcon = 0;//LoadIcon(NULL, IDI_ERROR);///The Top Left Icon of the WIndow;
	wc.hIconSm = 0;//LoadIcon(NULL, IDI_ASTERISK);///The Icon which is Shown on the Taskbar;
	wc.hInstance = NULL;//The hInstance parameter allows different instances of an application to be identified and managed, for example when you have to Microsoft edge webrowsers. There has the be a Way to Destingish one from another. 
	wc.lpszClassName = L"MyWindowClass";///The Name of the Application, THis Text is Shown on the top of the Window;
	wc.lpszMenuName = L"";//Use this to help specfiy what type of Menu you want on your Window.
	wc.style = CS_HREDRAW | CS_VREDRAW; ///Specifys how the WIndow Intracts with user for example should we Redraw the Window when we resized it vertically, USing the CS_VREDRAW command
	wc.lpfnWndProc = &WndProc;//Referencing the WndProc the a Function pointer, which will constantly call WndProc, allowing the the WndProc the check for events
	///If the registration of WIndow class fail, the function will return false;
	if (!::RegisterClassEx(&wc))
		throw std::exception("Failed to create window");


	///Creates a new window
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, this);

	///if the creation fail return false
	if (!m_hwnd)
		throw std::exception("Failed to create window");

	///Shows the Window Using the SW_SHOW Command in the form or a Macro
	::ShowWindow(m_hwnd, SW_SHOW);
	///This WIll Update the Window, ONe Reason WHy this Function is Needed is becuase it will help call the WndProc which checks for Events
	::UpdateWindow(m_hwnd);

	///set this flag to true to indicate that the window is initialized and running
	m_is_run = true;
}





//This Function will do a Broadcast to check for messages.
bool Window::broadcast()
{
	///Declarring/Creating a Varible Whcih Will hold the Messages from the Window
	MSG msg;//MSG is short for message.
		
	if(!this->m_is_init)
	{
		/// .. and then stored for later lookup
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_is_init = true;
	}

	this->onUpdate();
	///A message loop that continuously retrieves messages from the application's message queue
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		///Translates the Message for the next function to be able to read the Message
		TranslateMessage(&msg);
		//Runs some even lower level code in the backround to Help get the Message in the Parameters of the WnProc Function
		//Based on the Message this Could be used the trgger certian Window Events in the WnProc's Switch cases.
		DispatchMessage(&msg);
	}
	///Gives the Computer a 1 Milli-second to realeve some streess on the CPU a bit before conutinuing
	Sleep(1);

	return true;
}



//Return 1/True if the window is Running; If not it return 0/false 
bool Window::isRun()
{

	if (m_is_run) broadcast();
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	///The RECT Variable is a Structure that holds Four variables 
	///1. The integral position of the left side of the Window
	///2. The integral position of the right side of the Window
	///3. The integral position of the top of the Window
	///4. The integral position of the Bottom of the WIndow
	RECT rc;
	///Gets the Current RECT->TOP, BUTTOM, LEFT, RIGHT of the m_hwnd Variable and assigns that information to the rc varable
	::GetClientRect(/*ref Input*/this->m_hwnd, /*ref OutPut*/&rc);

	return rc;//Return RECT rc variable

}
RECT Window::getScreenSize()
{
	RECT rc = {};
	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	return rc;
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
	m_is_run = false;
}

void Window::onFocus()
{

}

void Window::onKillFocus()
{

}

void Window::onSize()
{

}

//Called to delete resoures when not needed
Window::~Window()
{
	//Destroy the window
	DestroyWindow(m_hwnd);
}