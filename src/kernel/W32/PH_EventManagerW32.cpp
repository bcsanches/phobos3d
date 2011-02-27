#include "W32/PH_EventManagerW32.h"

#include "PH_Exception.h"
#include "PH_InputActions.h"

#include <windows.h>
#include <windowsX.h>

namespace Phobos
{
	struct WinToKeyCode_s
	{
		UInt16_t u16Win;
		UInt16_t u16Phobos;
	};

	static WinToKeyCode_s stWinToKeyCode_g[] =
	{
		{0, 0},									//0x00
		{0, 0},									//0x01
		{0, 0},									//0x02
		{0, 0}, 								//0x03
		{0, 0},									//0x04
		{0, 0},									//0x05
		{0, 0},									//0x06
		{0, 0},									//0x07

		{VK_BACK,		KB_BACKSPACE},			//0x08
		{VK_TAB,		KB_TAB},				//0x09

		{0, 0},									//0x0A
		{0, 0},									//0x0B

		{VK_CLEAR, 		KB_CLEAR},				//0x0C
		{VK_RETURN, 	KB_ENTER},				//0x0D,

		{0, 0},									//0x0E
		{0, 0},									//0x0F

		{VK_SHIFT, 		KB_SHIFT},				//0x10
		{VK_CONTROL, 	KB_CONTROL},			//0x11
		{VK_MENU, 		KB_ALT},				//0x12
		{VK_PAUSE, 		KB_PAUSE},				//0x13
		{VK_CAPITAL, 	KB_CAPS_LOCK},			//0x14

		{0, 0},									//0x15
		{0, 0},									//0x16
		{0, 0},									//0x17
		{0, 0},									//0x18
		{0, 0},									//0x19
		{0, 0},									//0x1A

		{VK_ESCAPE, 	KB_ESCAPE},				//0x1B

		{0, 0},									//0x1C
		{0, 0},									//0x1D
		{0, 0},									//0x1E
		{0, 0},									//0x1F

		{VK_SPACE, 		KB_SPACE},				//0x20
		{VK_PRIOR, 		KB_PAGE_UP},			//0x21
		{VK_NEXT, 		KB_PAGE_DOWN},			//0x22
		{VK_END, 		KB_END},				//0x23
		{VK_HOME, 		KB_HOME},				//0x24
		{VK_LEFT,		KB_LEFT_ARROW},			//0X25
		{VK_UP,			KB_UP_ARROW},			//0X26
		{VK_RIGHT,		KB_RIGHT_ARROW},		//0X27
		{VK_DOWN,		KB_DOWN_ARROW},			//0X28

		{0, 0},									//0X29
		{0, 0},									//0X2A
		{0, 0},									//0X2B

		{VK_SNAPSHOT, 	KB_PRINT_SCREEN},		//0X2C
		{VK_INSERT, 	KB_INSERT},				//0X2D
		{VK_DELETE, 	KB_DELETE},				//0X2E

		{0, 0},									//0X2F

		{'0', 			'0'},					//0X30
		{'1', 			'1'},					//0X31
		{'2', 			'2'},					//0X32
		{'3', 			'3'},					//0X33
		{'4', 			'4'},					//0X34
		{'5', 			'5'},					//0X35
		{'6', 			'6'},					//0X36
		{'7', 			'7'},					//0X37
		{'8', 			'8'},					//0X38
		{'9', 			'9'},					//0X39

		{0, 0},									//0X3A
		{0, 0},									//0X3B
		{0, 0},									//0X3C
		{0, 0},									//0X3D
		{0, 0},									//0X3E
		{0, 0},									//0X3F
		{0, 0},									//0X40

		{'A',			'a'},					//0x41
		{'B',			'b'},					//0x42
		{'C',			'c'},					//0x43
		{'D',			'd'},					//0x44
		{'E',			'e'},					//0x45
		{'F',			'f'},					//0x46
		{'G',			'g'},					//0x47
		{'H',			'h'},					//0x48
		{'I',			'i'},					//0x49
		{'J',			'j'},					//0x4A
		{'K',			'k'},					//0x4B
		{'L',			'l'},					//0x4C
		{'M',			'm'},					//0x4D
		{'N',			'n'},					//0x4E
		{'O',			'o'},					//0x4F
		{'P',			'p'},					//0x50
		{'Q',			'q'},					//0x51
		{'R',			'r'},					//0x52
		{'S',			's'},					//0x53
		{'T',			't'},					//0x54
		{'U',			'u'},					//0x55
		{'V',			'v'},					//0x56
		{'W',			'w'},					//0x57
		{'X',			'x'},					//0x58
		{'Y',			'y'},					//0x59
		{'Z',			'z'},					//0x5A

		{0, 0},									//0X5B
		{0, 0},									//0X5C
		{0, 0},									//0X5D
		{0, 0},									//0X5E
		{0, 0},									//0X5F

		{VK_NUMPAD0, 	KB_KP_INSERT},			//0X60
		{VK_NUMPAD1, 	KB_KP_END},				//0X61
		{VK_NUMPAD2, 	KB_KP_DOWN_ARROW},		//0X62
		{VK_NUMPAD3, 	KB_KP_PAGE_DOWN},		//0X63
		{VK_NUMPAD4, 	KB_KP_LEFT_ARROW},		//0X64
		{VK_NUMPAD5, 	KB_KP_5},				//0X65
		{VK_NUMPAD6, 	KB_KP_RIGHT_ARROW},		//0X66
		{VK_NUMPAD7, 	KB_KP_HOME},			//0X67
		{VK_NUMPAD8, 	KB_KP_UP_ARROW},		//0X68
		{VK_NUMPAD9, 	KB_KP_PAGE_UP},			//0X69
		{VK_MULTIPLY, 	KB_KP_MUL},				//0X6A
		{VK_ADD, 		KB_KP_PLUS},			//0X6B
		{VK_SEPARATOR, 	KB_KP_ENTER},			//0X6C
		{VK_SUBTRACT, 	KB_KP_MINUS},			//0X6D
		{VK_DECIMAL, 	KB_KP_DELETE},			//0X6E
		{VK_DIVIDE, 	KB_KP_SLASH},			//0X6F

		{VK_F1, 		KB_F1},					//0X70
		{VK_F2, 		KB_F2},					//0X71
		{VK_F3,			KB_F3},					//0X72
		{VK_F4,			KB_F4},					//0X73

		{VK_F5, 		KB_F5},					//0X74
		{VK_F6,			KB_F6},					//0X75
		{VK_F7,			KB_F7},					//0X76
		{VK_F8,			KB_F8},					//0X77

		{VK_F9, 		KB_F9},					//0X78
		{VK_F10, 		KB_F10},				//0X79
		{VK_F11, 		KB_F11},				//0X7A
		{VK_F12,		KB_F12},				//0X7B

		{0, 0},									//0X7C
		{0, 0},									//0X7D
		{0, 0},									//0X7E
		{0, 0},									//0X7F
		{0, 0},									//0X80
		{0, 0},									//0X81
		{0, 0},									//0X82
		{0, 0},									//0X83
		{0, 0},									//0X84
		{0, 0},									//0X85
		{0, 0},									//0X86
		{0, 0},									//0X87
		{0, 0},									//0X88
		{0, 0},									//0X89
		{0, 0},									//0X8A
		{0, 0},									//0X8B
		{0, 0},									//0X8C
		{0, 0},									//0X8D
		{0, 0},									//0X8E
		{0, 0},									//0X8F

		{VK_NUMLOCK, 	KB_NUM_LOCK},			//0X90
		{VK_SCROLL, 	KB_SCROLL_LOCK},		//0X91

		{0, 0},									//0X92
		{0, 0},									//0X93
		{0, 0},									//0X94
		{0, 0},									//0X95
		{0, 0},									//0X96
		{0, 0},									//0X97
		{0, 0},									//0X98
		{0, 0},									//0X99
		{0, 0},									//0X9A
		{0, 0},									//0X9B
		{0, 0},									//0X9C
		{0, 0},									//0X9D
		{0, 0},									//0X9E
		{0, 0},									//0X9F

		{VK_LSHIFT, 	KB_LEFT_SHIFT},		//0XA0
		{VK_RSHIFT, 	KB_RIGHT_SHIFT},		//0XA1
		{VK_LCONTROL, 	KB_LEFT_CONTROL},	//0XA2
		{VK_RCONTROL, 	KB_RIGHT_CONTROL},	//0XA3

		{0, 0},									//0XA4
		{0, 0},									//0XA5
		{0, 0},									//0XA6
		{0, 0},									//0XA7
		{0, 0},									//0XA8
		{0, 0},									//0XA9
		{0, 0},									//0XAA
		{0, 0},									//0XAB
		{0, 0},									//0XAC
		{0, 0},									//0XAD
		{0, 0},									//0XAE
		{0, 0},									//0XAF
		{0, 0},									//0XB0
		{0, 0},									//0XB1
		{0, 0},									//0XB2
		{0, 0},									//0XB3
		{0, 0},									//0XB4
		{0, 0},									//0XB5
		{0, 0},									//0XB6
		{0, 0},									//0XB7
		{0, 0},									//0XB8
		{0, 0},									//0XB9
		{0, 0},									//0XBA
		{'=','='},								//0XBB
		{0, 0},									//0XBC
		{'-','-'},								//0XBD
		{0, 0},									//0XBE
		{0, 0},									//0XBF
		{'`', '`'}								//0XC0
	};

	//
	//Creation HACK
	//
	static EventManagerPtr_t CreateInstanceLocalImpl(const String_c &name)
	{
		return EventManagerPtr_t(new EventManagerW32_c(name));
	}

	EventManagerPtr_t EventManager_c::CreateInstanceImpl(const String_c &name)
	{
		return CreateInstanceLocalImpl(name);
	}

	//
	//
	//LOCAL FUNCTIONS
	//
	//

	inline bool IsValidWinToPhobosKeyCode(WPARAM v)
	{
		if(v >= (sizeof(stWinToKeyCode_g) / sizeof(stWinToKeyCode_g[0])))
			return(false);

		if((stWinToKeyCode_g[v].u16Phobos == 0) && (stWinToKeyCode_g[v].u16Win == 0))
			return(false);

		return(true);
	}

	bool BuildKeyboardEvent(Event_s &event, MSG &msg)
	{
		event.eType = EVENT_TYPE_KEYBOARD;
		event.pParam = &msg;

		switch(msg.message)
		{
			case WM_KEYDOWN:
				//Is it a repeat key?
				if(msg.lParam & (1 << 30))
					return(false);

				if(!IsValidWinToPhobosKeyCode(msg.wParam))
					return(false);

				event.stKeyboard.eType = KEYBOARD_KEY_DOWN;
				event.stKeyboard.u16Code = (UInt16_t) stWinToKeyCode_g[msg.wParam].u16Phobos;
				break;

			case WM_KEYUP:
				if(!IsValidWinToPhobosKeyCode(msg.wParam))
					return(false);

				event.stKeyboard.eType = KEYBOARD_KEY_UP;
				event.stKeyboard.u16Code = (UInt16_t) stWinToKeyCode_g[msg.wParam].u16Phobos;
				break;

			case WM_CHAR:
				event.stKeyboard.eType = KEYBOARD_CHAR;
				event.stKeyboard.u16Code = (UInt16_t) msg.wParam;
				break;
		}

		return true;
	}

	void BuildMouseEvent(Event_s &event, MSG &msg)
	{
		event.eType = EVENT_TYPE_MOUSE;
		event.pParam = &msg;

		switch(msg.message)
		{
			case WM_MOUSEMOVE:
				event.stMouse.eType = MOUSE_MOVE;
				event.stMouse.u16ButtonId = MOUSE_THUMB;
				event.stMouse.u16X = GET_X_LPARAM(msg.lParam);
				event.stMouse.u16Y = GET_Y_LPARAM(msg.lParam);
				break;

			case WM_LBUTTONDOWN:
				event.stMouse.u16ButtonId = MOUSE_LBUTTON;
				event.stMouse.eType = MOUSE_BUTTON_DOWN;
				break;

			case WM_LBUTTONUP:
				event.stMouse.u16ButtonId = MOUSE_LBUTTON;
				event.stMouse.eType = MOUSE_BUTTON_UP;
				break;

			case WM_RBUTTONDOWN:
				event.stMouse.u16ButtonId = MOUSE_RBUTTON;
				event.stMouse.eType = MOUSE_BUTTON_DOWN;
				break;

			case WM_RBUTTONUP:
				event.stMouse.u16ButtonId = MOUSE_RBUTTON;
				event.stMouse.eType = MOUSE_BUTTON_UP;
				break;

			case WM_MBUTTONDOWN:
				event.stMouse.u16ButtonId = MOUSE_MBUTTON;
				event.stMouse.eType = MOUSE_BUTTON_DOWN;
				break;

			case WM_MBUTTONUP:
				event.stMouse.u16ButtonId = MOUSE_MBUTTON;
				event.stMouse.eType = MOUSE_BUTTON_UP;
				break;
		}
	}

	void BuildSystemEvent(Event_s &event, MSG &msg)
	{
		event.eType = EVENT_TYPE_SYSTEM;
		event.pParam = &msg;

		switch(msg.message)
		{
			case WM_QUIT:
				event.stSystem.eType = SYSTEM_QUIT;
				break;

			case WM_ACTIVATEAPP:
				event.stSystem.eType = SYSTEM_ACTIVATE;
				event.stSystem.fActive = LOWORD(msg.wParam) != WA_INACTIVE;
				event.stSystem.fMinimized = HIWORD(msg.wParam) ? true : false;
				break;
		}
	}

	//
	//
	//EventManagerW32
	//
	//

	EventManagerW32_c::EventManagerW32_c(const String_c &name):
		EventManager_c(name)
	{
		//empty
	}

	void EventManagerW32_c::OnWindowMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
			case WM_ACTIVATE:
				Event_s	event;
				event.eType = EVENT_TYPE_SYSTEM;
				event.pParam = NULL;
				event.stSystem.eType = SYSTEM_ACTIVATE;
				event.stSystem.fActive = LOWORD(wParam) != WA_INACTIVE;
				event.stSystem.fMinimized = HIWORD(wParam) ? true : false;

				//dispatch to all handlers
				this->NotityListeners(event);
				break;
		}
	}

	void EventManagerW32_c::Update()
	{
		Event_s	event;
		MSG		msg;

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch(msg.message)
			{
				case WM_CLOSE:
					PostQuitMessage(0);
					break;

				case WM_QUIT:
				case WM_ACTIVATE:
				case WM_ACTIVATEAPP:
					BuildSystemEvent(event, msg);
					this->NotityListeners(event);
					break;

				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_CHAR:
					if(!BuildKeyboardEvent(event, msg))
						continue;

					this->NotityListeners(event);
					break;

				case WM_MOUSEMOVE:
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
					BuildMouseEvent(event, msg);
					this->NotityListeners(event);
					break;
			}
		}
	}
}
