#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>
#include <Windows.h>
void Game::run ( )
{
	onCreate ( );
	
	///Declarring/Creating a Varible Whcih Will hold the Messages from the Window
	MSG msg = {};//MSG is short for message.

	while (m_isRunning) 
	{
		///A message loop that continuously retrieves messages from the application's message queue
		if (::PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			if(msg.message == WM_QUIT)
			{
				m_isRunning = false;
				continue;
			}
			else
			{
				///Translates the Message for the next function to be able to read the Message
				TranslateMessage ( &msg );
				//Runs some even lower level code in the backround to Help get the Message in the Parameters of the WnProc Function
				//Based on the Message this Could be used the trgger certian Window Events in the WnProc's Switch cases.
				DispatchMessage ( &msg );
			}
		}
		onInternalUpdate ( );
		
	}

	onQuit ( );
}





