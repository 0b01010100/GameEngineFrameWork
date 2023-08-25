/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2022, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/


//This is Very import file. This Contans most of the Functions and Variable types need the Set up a Window Application
#include "Window.h"

//Window* window=nullptr;
//Not Using this Constructor Function becuase  the time it who be called is the worng time it would be needed to make the Program Run smootly.
Window::Window()
{
	///Empty
}


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
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		/// .. and then stored for later lookup
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		//Set the Global hwnd to equal to the local hwnd.
		window->setHWND(hwnd);
		//Calls the ON create Event for Higher level code to be Excuted.
		window->onCreate();
		break;//End of Case
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
	///If none Of the Events are meet or Some Other Event is meet Which is not Listed; For Example WM_MOVING whcih Triggers a Even when the USer moves the WIndow.
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);///Returning the data Back to let the Processor know that the Events Weren't Met;
	}

	return NULL;///Returning Number zero, Letting the the Computer Know that the Events in the Swicth were met;
}

//This Funtion is Called For Initializing/Setting up the Window.
bool Window::init()
{
	///Setting up WNDCLASSEX object
	WNDCLASSEX wc;//Creating a Wondow Variable 
///Below a Customizations of the Window WHich many be shown Visually on the Screen.
	wc.cbClsExtra = NULL;// This is Only Used if Extra Computer Memory should be used for the class, in bytes
	wc.cbSize = sizeof(WNDCLASSEX);//Size of the Window Class
	wc.cbWndExtra = NULL;//Extra memory to allocate for each window Instance of this Window class type, in bytes.
	wc.hbrBackground = (HBRUSH)COLOR_MENUBAR;///Color Of Window;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);///Type of Mouse Cursor;
	wc.hIcon = LoadIcon(NULL, IDI_ERROR);///The Top Left Icon of the WIndow;
	wc.hIconSm = LoadIcon(NULL, IDI_ASTERISK);///The Icon which is Shown on the Taskbar;
	wc.hInstance = NULL;//The hInstance parameter allows different instances of an application to be identified and managed, for example when you have to Microsoft edge webrowsers. There has the be a Way to Destingish one from another. 
	wc.lpszClassName = L"MyWindowClass";///The Name of the Application, THis Text is Shown on the top of the Window;
	wc.lpszMenuName = L"";//Use this to help specfiy what type of Menu you want on your Window.
	wc.style = CS_HREDRAW | CS_VREDRAW; ///Specifys how the WIndow Intracts with user for example should we Redraw the Window when we resized it vertically, USing the CS_VREDRAW command
	wc.lpfnWndProc = &WndProc;//Referencing the WndProc the a Function pointer, which will constantly call WndProc, allowing the the WndProc the check for events
	///If the registration of WIndow class fail, the function will return false;
	if (!::RegisterClassEx(&wc)) 
		return false;


	///Creates a new window
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, this);

	///if the creation fail return false
	if (!m_hwnd)
		return false;

	///Shows the Window Using the SW_SHOW Command in the form or a Macro
	::ShowWindow(m_hwnd, SW_SHOW);
	///This WIll Update the Window, ONe Reason WHy this Function is Needed is becuase it will help call the WndProc which checks for Events
	::UpdateWindow(m_hwnd);

	///set this flag to true to indicate that the window is initialized and running
	m_is_run = true;


	///Returns True if All Commands were FulFilled Correctly
	return true;
}
//This Function will do a Broadcast to check for messages.
bool Window::broadcast()
{
	///Declarring/Creating a Varible Whcih Will hold the Messages from the Window
	MSG msg;//MSG is short for message.
	///Calls the Update Function, This Functions Is Just created for Convence so USe COders can seperat High level code from lower level code.
		///-Low Level Code is Code the Works behied the Sences and is mostly likly harder ths understand.
		///-Low Level Code is just Basic code and Ez to Understand.*/
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

//This called when the user wants to deleate or close the Window
bool Window::release()
{
	//Destroy the window
	if (!::DestroyWindow(m_hwnd))
		return false;

	return true;
}
//Return 1/True if the window is Running; If not it return 0/false 
bool Window::isRun()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	///The RECT Variable is a Structure that holds Four variables 
	///1. The integral hight of the left side of the Window
	///2. The integral hight of the right side of the Window
	///3. The integral Width of the top of the Window
	///4. The integral Width of the Bottom of the WIndow
	RECT rc;
	///Gets the Current RECT->TOP, BUTTOM, LEFT, RIGHT of the m_hwnd Variable and assigns that information to the rc varable
	::GetClientRect(/*ref Input*/this->m_hwnd, /*ref OutPut*/&rc);

	return rc;//Return RECT rc variable

}
//Sets the Gloable Window Handler to = the Local Window Handler 
void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
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
//Not Using this Destructor FUnction becuase  the time it who be called is the worng time it would be needed to make the Program Run smootly.
Window::~Window()
{
}