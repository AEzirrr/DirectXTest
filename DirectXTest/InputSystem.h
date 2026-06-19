#pragma once
#include "InputListener.h"
#include <map>
#include "Point.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();
	
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& pos);
	void showCursor(bool show);


public:
	static InputSystem* getInstance();

private:
	std::map<InputListener*, InputListener*> m_listeners;
	unsigned char m_keyboard_state[256] = {};
	unsigned char m_old_keyboard_state[256] = {};
	Point m_old_mouse_pos;
	bool m_first_time = true;
};

