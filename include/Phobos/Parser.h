/*
Phobos 3d
January 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_PARSER_H
#define PH_PARSER_H

#include "Phobos/Types.h"
#include "Phobos/String.h"

#include <istream>

namespace Phobos
{
	enum ParserTokens_e
	{
		TOKEN_NUMBER,
		TOKEN_STRING,
		TOKEN_ID,
		TOKEN_OPEN_BRACE,
		TOKEN_CLOSE_BRACE,
		TOKEN_OPEN_PAREN,
		TOKEN_CLOSE_PAREN,
		TOKEN_EQUAL,
		TOKEN_SEMI_COLON,
		TOKEN_COLON,
		TOKEN_ERROR,
		TOKEN_EOF,
		TOKEN_DOT
	};

	class PH_BASE_API Parser
	{
		public:
			static const Char_t *GetTokenTypeName(ParserTokens_e token);

		public:
			Parser(void);
			~Parser(void);

			void SetStream(std::istream *stream);

			ParserTokens_e GetToken(String_t *out);

			inline void PushToken(void);

			inline unsigned int GetCurrentLine() const;

		private:
			void SetLookAhead(Char_t ch);

			bool GetNextChar(Char_t &out);

		private:
			std::istream	*m_pclStream;
			String_t		m_strToken;
			ParserTokens_e	m_eTokenType;

			Char_t		m_chLookAhead;
			bool		m_fLookAhead;
			bool		m_fTokenAhead;

			unsigned int	m_uCurrentLine;
	};

	inline void Parser::PushToken(void)
	{
		m_fTokenAhead = true;
	}

	inline unsigned int Parser::GetCurrentLine() const
	{
		return m_uCurrentLine;
	}
}

#endif
