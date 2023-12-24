#include "InputSystem.h"
#include <Windows.h>

InputSystem* InputSystem::m_system = nullptr;

InputSystem::InputSystem()
{
	
}


InputSystem::~InputSystem()
{
	InputSystem::m_system = nullptr;
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};

	::GetCursorPos(&current_mouse_pos);

	if(m_first_time)
	{
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if(current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y)
	{
		//IF MOUSE POSITION IS DIFFERENT THAN THE PREVIOUS POSTION WE HAVE A onMOUSEMOVE Event
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
 
			(*it)->onMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
	for (unsigned int i = 0; i < 256; i++)
	{
		m_keys_state[i] = ::GetAsyncKeyState ( i );
		// KEY IS DOWN
		if (m_keys_state[i] & 0x8001)
		{
			std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

			while (it != m_set_listeners.end())
			{
				if(i == VK_LBUTTON)
				{
					if (m_keys_state[i] != m_old_keys_state[i]) 
						(*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
				}
				else if(i == VK_RBUTTON)
				{
					if (m_keys_state[i] != m_old_keys_state[i]) 
						(*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
				}
				else
				{		
					(*it)->onKeyDown(i);	
				}
				++it;
			}
		}
		else // KEY IS UP
		{
			if (m_keys_state[i] != m_old_keys_state[i])
			{
				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

				while (it != m_set_listeners.end())
				{
					if(i == VK_LBUTTON)
					{
						(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON)
					{
						(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else
						(*it)->onKeyUp(i);
				++it;
				}
			}

		}

	}
	// store current keys state to old keys state buffer
	::memcpy(m_old_keys_state, m_keys_state, sizeof(short) * 256);
}

void InputSystem::addListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	m_set_listeners.erase(listener);
}

void InputSystem::SetCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}

void InputSystem::create()
{
	if (InputSystem::m_system) throw std::exception("Input System was already created");
	InputSystem::m_system = new InputSystem();
}

void InputSystem::release()
{
	if (!InputSystem::m_system) return;
	delete InputSystem::m_system;
}
