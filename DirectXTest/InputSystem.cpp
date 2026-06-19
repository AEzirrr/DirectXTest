#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{}

InputSystem::~InputSystem()
{}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_first_time) {
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if(current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y) {

		std::map<InputListener*, InputListener*>::iterator it = m_listeners.begin();

		while (it != m_listeners.end())
		{
			it->second->onMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (::GetKeyboardState(m_keyboard_state)) {
		for (unsigned int i = 0; i < 256; ++i) {

			// key is down
			if (m_keyboard_state[i] & 0x80) { 

				std::map<InputListener*, InputListener*>::iterator it = m_listeners.begin();

				while (it != m_listeners.end())
				{
					if (i == VK_LBUTTON) {
						if(m_keyboard_state[i] != m_old_keyboard_state[i]) 
							it->second->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON) {
						if (m_keyboard_state[i] != m_old_keyboard_state[i])
							it->second->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else {
						it->second->onKeyDown(i);
					}

					++it;
				}
			}
			//key is up
			else {

				if (m_keyboard_state[i] != m_old_keyboard_state[i]) {

					std::map<InputListener*, InputListener*>::iterator it = m_listeners.begin();
					while (it != m_listeners.end())
					{
						if (i == VK_LBUTTON) {
							if (m_keyboard_state[i] != m_old_keyboard_state[i])
								it->second->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == VK_RBUTTON) {
							if (m_keyboard_state[i] != m_old_keyboard_state[i])
								it->second->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else{
							it->second->onKeyUp(i);
						}
						
						++it;
					}
				}
			}
		}
		::memcpy(m_old_keyboard_state, m_keyboard_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener * listener)
{
	m_listeners.insert(std::make_pair<InputListener*, InputListener*>(std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)));
}

void InputSystem::removeListener(InputListener * listener)
{
	std::map<InputListener*, InputListener*>::iterator it = m_listeners.find(listener);

	if (it != m_listeners.end())
	{
		m_listeners.erase(it);
	}
}

void InputSystem::setCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}


InputSystem* InputSystem::getInstance()
{
	static InputSystem instance;
	return &instance;
}
