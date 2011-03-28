#include "SDL/PH_EventManagerSDL.h"

#include "PH_Error.h"
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
	    {0, 0},									//0
	    {0, 0},									//1
	    {0, 0},									//2
	    {0, 0},									//3
	    {0, 0},									//4
	    {0, 0},									//5
	    {0, 0},									//6
		{0, 0},									//7

        {SDLK_BACKSPACE,	 KB_BACKSPACE},		//8
        {SDLK_TAB,		     KB_TAB},			//9

        {0, 0},									//10
        {0, 0},									//11

        {SDLK_CLEAR,		 KB_CLEAR},			//12
        {SDLK_RETURN,		 KB_ENTER},			//13

        {0, 0},									//14
        {0, 0},									//15
        {0, 0},									//16
        {0, 0},									//17
        {0, 0},									//18
        {SDLK_PAUSE,		 KB_PAUSE},			//19

        {0, 0},									//20
        {0, 0},									//21
        {0, 0},									//22
        {0, 0},									//23
        {0, 0},									//24
        {0, 0},									//25
        {0, 0},									//26
        {SDLK_ESCAPE,		 KB_ESCAPE},		//27

        {0, 0},									//28
        {0, 0},									//29
        {0, 0},									//30
        {0, 0},									//31

        {SDLK_SPACE,		 KB_SPACE},			//32
        {SDLK_EXCLAIM,		 0},				//33
        {SDLK_QUOTEDBL,		 0},				//34
        {SDLK_HASH,		     0},				//35
        {SDLK_DOLLAR,		 0},				//36

        {0, 0},									//37

        {SDLK_AMPERSAND,	0},					//38
        {SDLK_QUOTE,		0},					//39
        {SDLK_LEFTPAREN,	0},					//40
        {SDLK_RIGHTPAREN,	0},					//41
        {SDLK_ASTERISK,		0},					//42
        {SDLK_PLUS,		    0},					//43
        {SDLK_COMMA,		0},					//44
        {SDLK_MINUS,		0},					//45
        {SDLK_PERIOD,		0},					//46
        {SDLK_SLASH,		0},					//47
        {SDLK_0,	    '0'},					//48
        {SDLK_1,	    '1'},					//49
        {SDLK_2,		'2'},					//50
        {SDLK_3,		'3'},					//51
        {SDLK_4,		'4'},					//52
        {SDLK_5,		'5'},					//53
        {SDLK_6,		'6'},					//54
        {SDLK_7,		'7'},					//55
        {SDLK_8,		'8'},					//56
        {SDLK_9,		'9'},					//57

        {SDLK_COLON,		 0},				//58
        {SDLK_SEMICOLON,	 0},				//59
        {SDLK_LESS,		     0},				//60
        {SDLK_EQUALS,		 0},				//61
        {SDLK_GREATER,		 0},				//62
        {SDLK_QUESTION,		 0},				//63
        {SDLK_AT	,		 0},				//54


        {0, 0},									//55
        {0, 0},									//56
        {0, 0},									//57
        {0, 0},									//58
        {0, 0},									//59
        {0, 0},									//60
        {0, 0},									//61
        {0, 0},									//62
        {0, 0},									//63
        {0, 0},									//64
        {0, 0},									//65
        {0, 0},									//66
        {0, 0},									//67
        {0, 0},									//68
        {0, 0},									//69
        {0, 0},									//70
        {0, 0},									//71
        {0, 0},									//72
        {0, 0},									//73
        {0, 0},									//74
        {0, 0},									//75
        {0, 0},									//76
        {0, 0},									//77
        {0, 0},									//78
        {0, 0},									//79
        {0, 0},									//80
        {0, 0},									//81
        {0, 0},									//82
		{0, 0},									//83
		{0, 0},									//84
		{0, 0},									//85
		{0, 0},									//86
		{0, 0},									//87
		{0, 0},									//88
		{0, 0},									//89
		{0, 0},									//90

        /*
           Skip uppercase letters
         */
        {SDLK_LEFTBRACKET,      0},				//91
        {SDLK_BACKSLASH,		0},				//92
        {SDLK_RIGHTBRACKET,     0},				//93
        {SDLK_CARET,		    0},				//94
        {SDLK_UNDERSCORE,		0},				//95
        {SDLK_BACKQUOTE,		0},				//96
        {SDLK_a,		 'a'},					//97
        {SDLK_b,		 'b'},					//98
        {SDLK_c,		 'c'},					//99
        {SDLK_d,		 'd'},					//100
        {SDLK_e,		 'e'},					//101
        {SDLK_f,		 'f'},					//102
        {SDLK_g,		 'g'},					//103
        {SDLK_h,		 'h'},					//104
        {SDLK_i,		 'i'},					//105
        {SDLK_j,		 'j'},					//106
        {SDLK_k,		 'k'},					//107
        {SDLK_l,		 'l'},					//108
        {SDLK_m,		 'm'},					//109
        {SDLK_n,		 'n'},					//110
        {SDLK_o,		 'o'},					//111
        {SDLK_p,		 'p'},					//112
        {SDLK_q,		 'q'},					//113
        {SDLK_r,		 'r'},					//114
        {SDLK_s,		 's'},					//115
        {SDLK_t,		 't'},					//116
        {SDLK_u,		 'u'},					//117
        {SDLK_v,		 'v'},					//118
        {SDLK_w,		 'w'},					//119
        {SDLK_x,		 'x'},					//120
        {SDLK_y,		 'y'},					//121
        {SDLK_z,		 'z'},					//122

        {0, 0},									//123
        {0, 0},									//124
        {0, 0},									//125
        {0, 0},									//126

        {SDLK_DELETE,		 0},				//127

		{0, 0},									//128
		{0, 0},									//129
		{0, 0},									//130
		{0, 0},									//131
		{0, 0},									//132
		{0, 0},									//133
		{0, 0},									//134
		{0, 0},									//135
		{0, 0},									//136
		{0, 0},									//137
		{0, 0},									//138
		{0, 0},									//139
		{0, 0},									//140
		{0, 0},									//141
		{0, 0},									//142
		{0, 0},									//143
		{0, 0},									//144
		{0, 0},									//145
		{0, 0},									//146
		{0, 0},									//147
		{0, 0},									//148
		{0, 0},									//149
		{0, 0},									//150
		{0, 0},									//151
		{0, 0},									//152
		{0, 0},									//153
		{0, 0},									//154
		{0, 0},									//155
		{0, 0},									//156
		{0, 0},									//157
		{0, 0},									//158
		{0, 0},									//159

		{SDLK_WORLD_0,		 0},				//160
        {SDLK_WORLD_1,		 0},				//161
        {SDLK_WORLD_2,		 0},				//162
        {SDLK_WORLD_3,		 0},				//163
        {SDLK_WORLD_4,		 0},				//164
        {SDLK_WORLD_5,		 0},				//165
        {SDLK_WORLD_6,		 0},				//166
        {SDLK_WORLD_7,		 0},				//167
        {SDLK_WORLD_8,		 0},				//168
        {SDLK_WORLD_9,		 0},				//169
        {SDLK_WORLD_10,		 0},				//170
        {SDLK_WORLD_11,		 0},				//171
        {SDLK_WORLD_12,		 0},				//172
        {SDLK_WORLD_13,		 0},				//173
        {SDLK_WORLD_14,		 0},				//174
        {SDLK_WORLD_15,		 0},				//175
        {SDLK_WORLD_16,		 0},				//176
        {SDLK_WORLD_17,		 0},				//177
        {SDLK_WORLD_18,		 0},				//178
        {SDLK_WORLD_19,		 0},				//179
        {SDLK_WORLD_20,		 0},				//180
        {SDLK_WORLD_21,		 0},				//181
        {SDLK_WORLD_22,		 0},				//182
        {SDLK_WORLD_23,		 0},				//183
        {SDLK_WORLD_24,		 0},				//184
        {SDLK_WORLD_25,		 0},				//185
        {SDLK_WORLD_26,		 0},				//186
        {SDLK_WORLD_27,		 0},				//187
        {SDLK_WORLD_28,		 0},				//188
        {SDLK_WORLD_29,		 0},				//189
        {SDLK_WORLD_30,		 0},				//190
        {SDLK_WORLD_31,		 0},				//191
        {SDLK_WORLD_32,		 0},				//192
        {SDLK_WORLD_33,		 0},				//193
        {SDLK_WORLD_34,		 0},				//194
        {SDLK_WORLD_35,		 0},				//195
        {SDLK_WORLD_36,		 0},				//196
        {SDLK_WORLD_37,		 0},				//197
        {SDLK_WORLD_38,		 0},				//198
        {SDLK_WORLD_39,		 0},				//199
        {SDLK_WORLD_40,		 0},				//200
        {SDLK_WORLD_41,		 0},				//201
        {SDLK_WORLD_42,		 0},				//202
        {SDLK_WORLD_43,		 0},				//203
        {SDLK_WORLD_44,		 0},				//204
        {SDLK_WORLD_45,		 0},				//205
        {SDLK_WORLD_46,		 0},				//206
        {SDLK_WORLD_47,		 0},				//207
        {SDLK_WORLD_48,		 0},				//208
        {SDLK_WORLD_49,		 0},				//209
        {SDLK_WORLD_50,		 0},				//210
        {SDLK_WORLD_51,		 0},				//211
        {SDLK_WORLD_52,		 0},				//212
        {SDLK_WORLD_53,		 0},				//213
        {SDLK_WORLD_54,		 0},				//214
        {SDLK_WORLD_55,		 0},				//215
        {SDLK_WORLD_56,		 0},				//216
        {SDLK_WORLD_57,		 0},				//217
        {SDLK_WORLD_58,		 0},				//218
        {SDLK_WORLD_59,		 0},				//219
        {SDLK_WORLD_60,		 0},				//220
        {SDLK_WORLD_61,		 0},				//221
        {SDLK_WORLD_62,		 0},				//222
        {SDLK_WORLD_63,		 0},				//223
        {SDLK_WORLD_64,		 0},				//224
        {SDLK_WORLD_65,		 0},				//225
        {SDLK_WORLD_66,		 0},				//226
        {SDLK_WORLD_67,		 0},				//227
        {SDLK_WORLD_68,		 0},				//228
        {SDLK_WORLD_69,		 0},				//229
        {SDLK_WORLD_70,		 0},				//230
        {SDLK_WORLD_71,		 0},				//231
        {SDLK_WORLD_72,		 0},				//232
        {SDLK_WORLD_73,		 0},				//233
        {SDLK_WORLD_74,		 0},				//234
        {SDLK_WORLD_75,		 0},				//235
        {SDLK_WORLD_76,		 0},				//236
        {SDLK_WORLD_77,		 0},				//237
        {SDLK_WORLD_78,		 0},				//238
        {SDLK_WORLD_79,		 0},				//239
        {SDLK_WORLD_80,		 0},				//240
        {SDLK_WORLD_81,		 0},				//241
        {SDLK_WORLD_82,		 0},				//242
        {SDLK_WORLD_83,		 0},				//243
        {SDLK_WORLD_84,		 0},				//244
        {SDLK_WORLD_85,		 0},				//245
        {SDLK_WORLD_86,		 0},				//246
        {SDLK_WORLD_87,		 0},				//247
        {SDLK_WORLD_88,		 0},				//248
        {SDLK_WORLD_89,		 0},				//249
        {SDLK_WORLD_90,		 0},				//250
        {SDLK_WORLD_91,		 0},				//251
        {SDLK_WORLD_92,		 0},				//252
        {SDLK_WORLD_93,		 0},				//253
        {SDLK_WORLD_94,		 0},				//254
        {SDLK_WORLD_95,		 0},				//255


        {SDLK_KP0,		 KB_KP_INSERT},			//256
        {SDLK_KP1,		 KB_KP_END},			//257
        {SDLK_KP2,		 KB_KP_DOWN_ARROW},		//258
        {SDLK_KP3,		 KB_KP_PAGE_DOWN},		//259
        {SDLK_KP4,		 KB_KP_LEFT_ARROW},		//260
        {SDLK_KP5,		 KB_KP_5},				//261
        {SDLK_KP6,		 KB_KP_RIGHT_ARROW},	//262
        {SDLK_KP7,		 KB_KP_HOME},			//263
        {SDLK_KP8,		 KB_KP_UP_ARROW},		//264
        {SDLK_KP9,		 KB_KP_PAGE_UP},		//265

        {SDLK_KP_PERIOD,		'.'},			//266
        {SDLK_KP_DIVIDE,	 KB_KP_SLASH},		//267
        {SDLK_KP_MULTIPLY,   KB_KP_MUL},		//268
        {SDLK_KP_MINUS,		 KB_KP_MINUS},		//269
        {SDLK_KP_PLUS,		 KB_KP_PLUS},		//270
        {SDLK_KP_ENTER,		 KB_KP_ENTER},		//271
        {SDLK_KP_EQUALS,		 '='},			//272


        {SDLK_UP,		 KB_UP_ARROW},			//273
        {SDLK_DOWN,		 KB_DOWN_ARROW},		//274
        {SDLK_RIGHT,	 KB_RIGHT_ARROW},		//275
        {SDLK_LEFT,		 KB_LEFT_ARROW},		//276
        {SDLK_INSERT,	 KB_INSERT},			//277
        {SDLK_HOME,		 KB_HOME},				//278
        {SDLK_END,		 KB_END},				//279
        {SDLK_PAGEUP,	 KB_PAGE_UP},			//280
        {SDLK_PAGEDOWN,	 KB_PAGE_DOWN},			//281


        {SDLK_F1,		 KB_F1},				//282
        {SDLK_F2,		 KB_F2},				//283
        {SDLK_F3,		 KB_F3},				//284
        {SDLK_F4,		 KB_F4},				//285
        {SDLK_F5,		 KB_F5},				//286
        {SDLK_F6,		 KB_F6},				//287
        {SDLK_F7,		 KB_F7},				//288
        {SDLK_F8,		 KB_F8},				//289
        {SDLK_F9,		 KB_F9},				//290
        {SDLK_F10,		 KB_F10},				//291
        {SDLK_F11,		 KB_F11},				//292
        {SDLK_F12,		 KB_F12},				//293
        {SDLK_F13,		 0},					//294
        {SDLK_F14,		 0},					//295
        {SDLK_F15,		 0},					//296

        {0, 0},									//297
        {0, 0},									//298
        {0, 0},									//299


        {SDLK_NUMLOCK,		 KB_NUM_LOCK},		//300
        {SDLK_CAPSLOCK,		 KB_CAPS_LOCK},		//301
        {SDLK_SCROLLOCK,	 KB_SCROLL_LOCK},	//302
        {SDLK_RSHIFT,		 KB_RIGHT_SHIFT},	//303
        {SDLK_LSHIFT,		 KB_LEFT_SHIFT},	//304
        {SDLK_RCTRL,		 KB_RIGHT_CONTROL},	//305
        {SDLK_LCTRL,		 KB_LEFT_CONTROL},	//306
        {SDLK_RALT,		     KB_ALT},			//307 TODO: Left Right ?
        {SDLK_LALT,		     KB_ALT},			//308
        {SDLK_RMETA,		 0},				//309
        {SDLK_LMETA,		 0},				//310
        {SDLK_LSUPER,		 0},				//311	/**< Left "Windows" key */
        {SDLK_RSUPER,		 0},				//312	/**< Right "Windows" key */
        {SDLK_MODE,		     0},				//313	/**< "Alt Gr" key */
        {SDLK_COMPOSE,		 0},				//314	/**< Multi-key compose key */


        {SDLK_HELP,		 0},					//315
        {SDLK_PRINT,	 0},					//316
        {SDLK_SYSREQ,	 0},					//317
        {SDLK_BREAK,	 0},					//318
        {SDLK_MENU,		 0},					//319
        {SDLK_POWER,	 0},					//320		/**< Power Macintosh power key */
        {SDLK_EURO,		 0},					//321		/**< Some european keyboards */
        {SDLK_UNDO,		 0},					//322		/**< Atari keyboard has Undo */

        {SDLK_LAST, 0}							//323
	};

	//
	//Creation HACK
	//
	EventManagerPtr_t EventManager_c::CreateInstanceImpl(const String_c &name)
	{
		return EventManagerPtr_t(new EventManagerSDL_c(name));
	}

	//
	//
	//LOCAL FUNCTIONS
	//
	//

	inline bool EventManagerSDL_c::IsValidSDLToPhobosKeyCode(SDLKey key)
	{
		if(key >= (sizeof(stSDLToKeyCode_g) / sizeof(stSDLToKeyCode_g[0])))
			return(false);

		if((stSDLToKeyCode_g[key].u16Phobos == 0) && (stSDLToKeyCode_g[key].u16SDL == 0))
			return(false);

		return(true);
	}

	bool EventManagerSDL_c::BuildKeyboardEvent(Event_s &event, SDL_Event &sdl_event)
	{
		event.eType = EVENT_TYPE_KEYBOARD;
		event.pParam = &sdl_event;

		switch(sdl_event.type)
		{
			case SDL_KEYDOWN:

                if(sdl_event.key.keysym.unicode != 0)
                {
                    event.stKeyboard.eType   = KEYBOARD_CHAR;
                    event.stKeyboard.u16Code = sdl_event.key.keysym.unicode;

                    this->NotityListeners(event);
                }

                if(!IsValidSDLToPhobosKeyCode(sdl_event.key.keysym.sym))
                    return(false);

                event.stKeyboard.eType   = KEYBOARD_KEY_DOWN;
                event.stKeyboard.u16Code = (UInt16_t) stSDLToKeyCode_g[sdl_event.key.keysym.sym].u16Phobos;

				break;

			case SDL_KEYUP:
				if(!IsValidSDLToPhobosKeyCode(sdl_event.key.keysym.sym))
					return(false);

				event.stKeyboard.eType   = KEYBOARD_KEY_UP;
				event.stKeyboard.u16Code = (UInt16_t) stSDLToKeyCode_g[sdl_event.key.keysym.sym].u16Phobos;
				break;

            default:
                break;
		}

		return true;
	}

	void EventManagerSDL_c::BuildMouseEvent(Event_s &event, SDL_Event& sdl_event)
	{
		event.eType = EVENT_TYPE_MOUSE;
		event.pParam = &sdl_event;

		switch(sdl_event.type)
		{
			case SDL_MOUSEMOTION:

				event.stMouse.eType         = MOUSE_MOVE;
				event.stMouse.u16X          = sdl_event.motion.x;
				event.stMouse.u16Y          = sdl_event.motion.y;
                event.stMouse.u16ButtonId   = MOUSE_THUMB;

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

	void EventManagerSDL_c::BuildSystemEvent(Event_s &event, SDL_Event &sdl_event)
	{
        event.eType  = EVENT_TYPE_SYSTEM;
        event.pParam = &sdl_event;

		switch(sdl_event.type)
		{
			case SDL_QUIT:

				event.stSystem.eType = SYSTEM_QUIT;

				break;

			case SDL_ACTIVEEVENT:

				event.stSystem.eType = SYSTEM_ACTIVATE;
				event.stSystem.fActive = true;
				event.stSystem.fMinimized = false;

				break;

            default:
				PH_ASSERT(false);
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
