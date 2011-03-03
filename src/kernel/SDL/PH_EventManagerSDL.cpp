#include "SDL/PH_EventManagerSDL.h"

#include "PH_Exception.h"
#include "PH_InputActions.h"

namespace Phobos
{
	struct SDLToKeyCode_s
	{
		UInt16_t u16SDL;
		UInt16_t u16Phobos;
	};

	struct SDLToMouseButton_s
	{
	    UInt16_t u16SDL;
	    UInt16_t u16Phobos;
	};

	static SDLToMouseButton_s SDLToMouseButton_g[] =
	{
	    {0, 0},
	    {SDL_BUTTON_LEFT,   MOUSE_LBUTTON},
	    {SDL_BUTTON_MIDDLE, MOUSE_MBUTTON},
	    {SDL_BUTTON_RIGHT,  MOUSE_RBUTTON},

	    {0, 0},
	    {0, 0},
	    {0, 0},
	    {0, 0},
	};

	static SDLToKeyCode_s stSDLToKeyCode_g[] =
	{
	    {0, 0},
	    {0, 0},
	    {0, 0},
	    {0, 0},
	    {0, 0},
	    {0, 0},
	    {0, 0},

        {SDLK_BACKSPACE,	 KB_BACKSPACE},
        {SDLK_TAB,		     KB_TAB},

        {0, 0},
        {0, 0},

        {SDLK_CLEAR,		 KB_CLEAR},
        {SDLK_RETURN,		 KB_ENTER},

        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},

        {SDLK_PAUSE,		 KB_PAUSE},

        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},

        {SDLK_ESCAPE,		 KB_ESCAPE},

        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},

        {SDLK_SPACE,		 KB_SPACE},
        {SDLK_EXCLAIM,		 0},//TODO: possivel char '!'
        {SDLK_QUOTEDBL,		 0},
        {SDLK_HASH,		     0},
        {SDLK_DOLLAR,		 0},

        {0, 0},

        {SDLK_AMPERSAND,	0},
        {SDLK_QUOTE,		0},
        {SDLK_LEFTPAREN,	0},
        {SDLK_RIGHTPAREN,	0},
        {SDLK_ASTERISK,		0},
        {SDLK_PLUS,		    0},
        {SDLK_COMMA,		0},
        {SDLK_MINUS,		0},
        {SDLK_PERIOD,		0},
        {SDLK_SLASH,		0},
        {SDLK_0,	    '0'},
        {SDLK_1,	    '1'},
        {SDLK_2,		'2'},
        {SDLK_3,		'3'},
        {SDLK_4,		'4'},
        {SDLK_5,		'5'},
        {SDLK_6,		'6'},
        {SDLK_7,		'7'},
        {SDLK_8,		'8'},
        {SDLK_9,		'9'},

        {SDLK_COLON,		 0}, //TODO: possivel char
        {SDLK_SEMICOLON,	 0},
        {SDLK_LESS,		     0},
        {SDLK_EQUALS,		 0},
        {SDLK_GREATER,		 0},
        {SDLK_QUESTION,		 0},
        {SDLK_AT	,		 0},


        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},


        /*
           Skip uppercase letters
         */
        {SDLK_LEFTBRACKET,      0},
        {SDLK_BACKSLASH,		0},
        {SDLK_RIGHTBRACKET,     0},
        {SDLK_CARET,		    0},
        {SDLK_UNDERSCORE,		0},
        {SDLK_BACKQUOTE,		0},
        {SDLK_a,		 'a'},
        {SDLK_b,		 'b'},
        {SDLK_c,		 'c'},
        {SDLK_d,		 'd'},
        {SDLK_e,		 'e'},
        {SDLK_f,		 'f'},
        {SDLK_g,		 'g'},
        {SDLK_h,		 'h'},
        {SDLK_i,		 'i'},
        {SDLK_j,		 'j'},
        {SDLK_k,		 'k'},
        {SDLK_l,		 'l'},
        {SDLK_m,		 'm'},
        {SDLK_n,		 'n'},
        {SDLK_o,		 'o'},
        {SDLK_p,		 'p'},
        {SDLK_q,		 'q'},
        {SDLK_r,		 'r'},
        {SDLK_s,		 's'},
        {SDLK_t,		 't'},
        {SDLK_u,		 'u'},
        {SDLK_v,		 'v'},
        {SDLK_w,		 'w'},
        {SDLK_x,		 'x'},
        {SDLK_y,		 'y'},
        {SDLK_z,		 'z'}, //122

        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},


        {SDLK_DELETE,		 0}, //127


        {SDLK_WORLD_0,		 0},		/* 0xA0 */
        {SDLK_WORLD_1,		 0},
        {SDLK_WORLD_2,		 0},
        {SDLK_WORLD_3,		 0},
        {SDLK_WORLD_4,		 0},
        {SDLK_WORLD_5,		 0},
        {SDLK_WORLD_6,		 0},
        {SDLK_WORLD_7,		 0},
        {SDLK_WORLD_8,		 0},
        {SDLK_WORLD_9,		 0},
        {SDLK_WORLD_10,		 0},
        {SDLK_WORLD_11,		 0},
        {SDLK_WORLD_12,		 0},
        {SDLK_WORLD_13,		 0},
        {SDLK_WORLD_14,		 0},
        {SDLK_WORLD_15,		 0},
        {SDLK_WORLD_16,		 0},
        {SDLK_WORLD_17,		 0},
        {SDLK_WORLD_18,		 0},
        {SDLK_WORLD_19,		 0},
        {SDLK_WORLD_20,		 0},
        {SDLK_WORLD_21,		 0},
        {SDLK_WORLD_22,		 0},
        {SDLK_WORLD_23,		 0},
        {SDLK_WORLD_24,		 0},
        {SDLK_WORLD_25,		 0},
        {SDLK_WORLD_26,		 0},
        {SDLK_WORLD_27,		 0},
        {SDLK_WORLD_28,		 0},
        {SDLK_WORLD_29,		 0},
        {SDLK_WORLD_30,		 0},
        {SDLK_WORLD_31,		 0},
        {SDLK_WORLD_32,		 0},
        {SDLK_WORLD_33,		 0},
        {SDLK_WORLD_34,		 0},
        {SDLK_WORLD_35,		 0},
        {SDLK_WORLD_36,		 0},
        {SDLK_WORLD_37,		 0},
        {SDLK_WORLD_38,		 0},
        {SDLK_WORLD_39,		 0},
        {SDLK_WORLD_40,		 0},
        {SDLK_WORLD_41,		 0},
        {SDLK_WORLD_42,		 0},
        {SDLK_WORLD_43,		 0},
        {SDLK_WORLD_44,		 0},
        {SDLK_WORLD_45,		 0},
        {SDLK_WORLD_46,		 0},
        {SDLK_WORLD_47,		 0},
        {SDLK_WORLD_48,		 0},
        {SDLK_WORLD_49,		 0},
        {SDLK_WORLD_50,		 0},
        {SDLK_WORLD_51,		 0},
        {SDLK_WORLD_52,		 0},
        {SDLK_WORLD_53,		 0},
        {SDLK_WORLD_54,		 0},
        {SDLK_WORLD_55,		 0},
        {SDLK_WORLD_56,		 0},
        {SDLK_WORLD_57,		 0},
        {SDLK_WORLD_58,		 0},
        {SDLK_WORLD_59,		 0},
        {SDLK_WORLD_60,		 0},
        {SDLK_WORLD_61,		 0},
        {SDLK_WORLD_62,		 0},
        {SDLK_WORLD_63,		 0},
        {SDLK_WORLD_64,		 0},
        {SDLK_WORLD_65,		 0},
        {SDLK_WORLD_66,		 0},
        {SDLK_WORLD_67,		 0},
        {SDLK_WORLD_68,		 0},
        {SDLK_WORLD_69,		 0},
        {SDLK_WORLD_70,		 0},
        {SDLK_WORLD_71,		 0},
        {SDLK_WORLD_72,		 0},
        {SDLK_WORLD_73,		 0},
        {SDLK_WORLD_74,		 0},
        {SDLK_WORLD_75,		 0},
        {SDLK_WORLD_76,		 0},
        {SDLK_WORLD_77,		 0},
        {SDLK_WORLD_78,		 0},
        {SDLK_WORLD_79,		 0},
        {SDLK_WORLD_80,		 0},
        {SDLK_WORLD_81,		 0},
        {SDLK_WORLD_82,		 0},
        {SDLK_WORLD_83,		 0},
        {SDLK_WORLD_84,		 0},
        {SDLK_WORLD_85,		 0},
        {SDLK_WORLD_86,		 0},
        {SDLK_WORLD_87,		 0},
        {SDLK_WORLD_88,		 0},
        {SDLK_WORLD_89,		 0},
        {SDLK_WORLD_90,		 0},
        {SDLK_WORLD_91,		 0},
        {SDLK_WORLD_92,		 0},
        {SDLK_WORLD_93,		 0},
        {SDLK_WORLD_94,		 0},
        {SDLK_WORLD_95,		 0},		/* 0xFF */


        {SDLK_KP0,		 KB_KP_INSERT}, //256
        {SDLK_KP1,		 KB_KP_END},
        {SDLK_KP2,		 KB_KP_DOWN_ARROW},
        {SDLK_KP3,		 KB_KP_PAGE_DOWN},
        {SDLK_KP4,		 KB_KP_LEFT_ARROW},
        {SDLK_KP5,		 KB_KP_5},
        {SDLK_KP6,		 KB_KP_RIGHT_ARROW},
        {SDLK_KP7,		 KB_KP_HOME},
        {SDLK_KP8,		 KB_KP_UP_ARROW},
        {SDLK_KP9,		 KB_KP_PAGE_UP},

        {SDLK_KP_PERIOD,		'.'},
        {SDLK_KP_DIVIDE,	 KB_KP_SLASH},
        {SDLK_KP_MULTIPLY,   KB_KP_MUL},
        {SDLK_KP_MINUS,		 KB_KP_MINUS},
        {SDLK_KP_PLUS,		 KB_KP_PLUS},
        {SDLK_KP_ENTER,		 KB_KP_ENTER},
        {SDLK_KP_EQUALS,		 '='},


        {SDLK_UP,		 KB_UP_ARROW},
        {SDLK_DOWN,		 KB_DOWN_ARROW},
        {SDLK_RIGHT,	 KB_RIGHT_ARROW},
        {SDLK_LEFT,		 KB_LEFT_ARROW},
        {SDLK_INSERT,	 KB_INSERT},
        {SDLK_HOME,		 KB_HOME},
        {SDLK_END,		 KB_END},
        {SDLK_PAGEUP,	 KB_PAGE_UP},
        {SDLK_PAGEDOWN,	 KB_PAGE_DOWN},


        {SDLK_F1,		 KB_F1},
        {SDLK_F2,		 KB_F2},
        {SDLK_F3,		 KB_F3},
        {SDLK_F4,		 KB_F4},
        {SDLK_F5,		 KB_F5},
        {SDLK_F6,		 KB_F6},
        {SDLK_F7,		 KB_F7},
        {SDLK_F8,		 KB_F8},
        {SDLK_F9,		 KB_F9},
        {SDLK_F10,		 KB_F10},
        {SDLK_F11,		 KB_F11},
        {SDLK_F12,		 KB_F12},
        {SDLK_F13,		 0},
        {SDLK_F14,		 0},
        {SDLK_F15,		 0}, //296

        {0, 0},
        {0, 0},
        {0, 0},


        {SDLK_NUMLOCK,		 KB_NUM_LOCK}, //300
        {SDLK_CAPSLOCK,		 KB_CAPS_LOCK},
        {SDLK_SCROLLOCK,	 KB_SCROLL_LOCK},
        {SDLK_RSHIFT,		 KB_RIGHT_SHIFT},
        {SDLK_LSHIFT,		 KB_LEFT_SHIFT},
        {SDLK_RCTRL,		 KB_RIGHT_CONTROL},
        {SDLK_LCTRL,		 KB_LEFT_CONTROL},
        {SDLK_RALT,		     KB_ALT}, //TODO: Left Right ?
        {SDLK_LALT,		     KB_ALT},
        {SDLK_RMETA,		 0},
        {SDLK_LMETA,		 0},
        {SDLK_LSUPER,		 0},		/**< Left "Windows" key */
        {SDLK_RSUPER,		 0},		/**< Right "Windows" key */
        {SDLK_MODE,		     0},		/**< "Alt Gr" key */
        {SDLK_COMPOSE,		 0},		/**< Multi-key compose key */


        {SDLK_HELP,		 0},
        {SDLK_PRINT,	 0},
        {SDLK_SYSREQ,	 0},
        {SDLK_BREAK,	 0},
        {SDLK_MENU,		 0},
        {SDLK_POWER,	 0},		/**< Power Macintosh power key */
        {SDLK_EURO,		 0},		/**< Some european keyboards */
        {SDLK_UNDO,		 0},		/**< Atari keyboard has Undo */

        {SDLK_LAST, 0}
	};

	//
	//Creation HACK
	//
	EventManagerPtr_t CreateInstanceLocalImpl(const String_c &name)
	{
		return EventManagerPtr_t(new EventManagerSDL_c(name));
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

	inline bool IsValidSDLToPhobosKeyCode(SDLKey key)
	{
		if(key >= (sizeof(stSDLToKeyCode_g) / sizeof(stSDLToKeyCode_g[0])))
			return(false);

		if((stSDLToKeyCode_g[key].u16Phobos == 0) && (stSDLToKeyCode_g[key].u16SDL == 0))
			return(false);

		return(true);
	}

	bool BuildKeyboardEvent(Event_s &event, SDL_Event &sdl_event)
	{
		event.eType = EVENT_TYPE_KEYBOARD;
		event.pParam = &sdl_event;

		switch(sdl_event.type)
		{
			case SDL_KEYDOWN:

                if(sdl_event.key.keysym.unicode != 0)
                {
                    event.stKeyboard.eType = KEYBOARD_CHAR;
                    event.stKeyboard.u16Code = sdl_event.key.keysym.unicode;
                }
                else
                {
                    if(!IsValidSDLToPhobosKeyCode(sdl_event.key.keysym.sym))
                        return(false);

                    event.stKeyboard.eType = KEYBOARD_KEY_DOWN;
                    event.stKeyboard.u16Code = (UInt16_t) stSDLToKeyCode_g[sdl_event.key.keysym.sym].u16Phobos;
                }

				break;

			case SDL_KEYUP:
				if(!IsValidSDLToPhobosKeyCode(sdl_event.key.keysym.sym))
					return(false);

				event.stKeyboard.eType = KEYBOARD_KEY_UP;
				event.stKeyboard.u16Code = (UInt16_t) stSDLToKeyCode_g[sdl_event.key.keysym.sym].u16Phobos;
				break;

            default:
                break;
		}

		return true;
	}

	void BuildMouseEvent(Event_s &event, SDL_Event& sdl_event)
	{
		event.eType = EVENT_TYPE_MOUSE;
		event.pParam = &sdl_event;

		switch(sdl_event.type)
		{
			case SDL_MOUSEMOTION:

				event.stMouse.eType = MOUSE_MOVE;
				event.stMouse.u16X = sdl_event.motion.x;
				event.stMouse.u16Y = sdl_event.motion.y;
                event.stMouse.u16ButtonId = MOUSE_THUMB;

				break;

			case SDL_MOUSEBUTTONDOWN:

                event.stMouse.u16ButtonId = SDLToMouseButton_g[sdl_event.button.button].u16Phobos;
				event.stMouse.eType = MOUSE_BUTTON_DOWN;

				break;

            case SDL_MOUSEBUTTONUP:

                event.stMouse.u16ButtonId = SDLToMouseButton_g[sdl_event.button.button].u16Phobos;
				event.stMouse.eType = MOUSE_BUTTON_UP;

                break;


            default:
                break;
		}
	}

	void BuildSystemEvent(Event_s &event, SDL_Event& sdl_event)
	{
		event.eType = EVENT_TYPE_SYSTEM;
		event.pParam = &sdl_event;

		switch(sdl_event.type)
		{
			case SDL_QUIT:
				event.stSystem.eType = SYSTEM_QUIT;
				break;

			case SDL_APPMOUSEFOCUS:
			case SDL_APPACTIVE:
				event.stSystem.eType = SYSTEM_ACTIVATE;
				event.stSystem.fActive = true;
				event.stSystem.fMinimized = false;
				break;
		}
	}

	//
	//
	//EventManagerSDL
	//
	//

	EventManagerSDL_c::EventManagerSDL_c(const String_c &name):
		EventManager_c(name)
	{
		//empty
	}

	void EventManagerSDL_c::Update()
	{
		Event_s	event;
		SDL_Event sdl_event;

		while(SDL_PollEvent(&sdl_event))
		{
			switch(sdl_event.type)
			{

				case SDL_QUIT:
                case SDL_ACTIVEEVENT:
					BuildSystemEvent(event, sdl_event);
					this->NotityListeners(event);
					break;

				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if(!BuildKeyboardEvent(event, sdl_event))
						continue;

					this->NotityListeners(event);
					break;

				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					BuildMouseEvent(event, sdl_event);
					this->NotityListeners(event);
					break;
			}
		}
	}
}
