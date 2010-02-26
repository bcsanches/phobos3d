/*
Phobos 3d
  January 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_PARSER_H
#define PH_PARSER_H

#include "PH_Types.h"
#include "PH_String.h"

#include <ostream>

namespace Phobos
{
	enum ParserTokens_e
	{
		PH_TOKEN_NUMBER,
		PH_TOKEN_STRING,
		PH_TOKEN_ID,
		PH_TOKEN_OPEN_BRACE,
		PH_TOKEN_CLOSE_BRACE,
		PH_TOKEN_OPEN_PAREN,
		PH_TOKEN_CLOSE_PAREN,
		PH_TOKEN_ERROR,
		PH_TOKEN_EOF
	};

	class PH_Parser_c
	{
		public:
			static const Char_t *GetTokenTypeName(ParserTokens_e token);

		public:
			PH_Parser_c(void);
			~PH_Parser_c(void);

			void SetStream(std::istream *stream);

			ParserTokens_e GetToken(String_c *out);

			inline void PushToken(void);

		private:
			void SetLookAhead(Char_t ch);

			bool GetNextChar(Char_t &out);

		private:
			std::istream	*pclStream;
			String_c		strToken;
			ParserTokens_e	eTokenType;

			Char_t		chLookAhead;
			bool		fLookAhead;
			bool		fTokenAhead;
	};

	inline void PH_Parser_c::PushToken(void)
	{
		fTokenAhead = true;
	}
}

#endif
