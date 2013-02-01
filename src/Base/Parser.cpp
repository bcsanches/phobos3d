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
#include "Phobos/Parser.h"

#include "Phobos/Error.h"
#include <iostream>

namespace Phobos
{
	#define IS_DIGIT(X) (((X >= '0') && (X <= '9')))
	#define IS_NUMBER(X) (IS_DIGIT(X) || (X == '.'))	
	#define IS_HEX_EXTRA_DIGITS(X) (((X >= 'a') && (X <= 'f')) || ((X >= 'A') && (X <= 'F')))

	#define IS_ID_START(X) (((X >= 'a') && (X <= 'z')) || ((X >= 'A') && (X <= 'Z')) || (X == '_'))
	#define IS_ID(X) (IS_DIGIT(X) || IS_ID_START(X) || (X == '-'))
	#define IS_NUMBER_START(X) ((X == '-') || IS_NUMBER(X))

	Parser::Parser(void):
        m_pclStream(NULL),
		m_fLookAhead(false),
		m_fTokenAhead(false)
	{
		//empty
	}

	Parser::~Parser(void)
	{
		//empty
	}

	void Parser::SetStream(std::istream *stream)
	{
		m_pclStream = stream;
		m_uCurrentLine = 0;

		if(stream != NULL)
			(*stream) >> std::noskipws;
	}

	void Parser::SetLookAhead(Char_t ch)
	{
		m_chLookAhead = ch;
		m_fLookAhead = true;
	}

	bool Parser::GetNextChar(Char_t &out)
	{
		PH_ASSERT_VALID(this);
		PH_ASSERT_VALID(m_pclStream);

		if(m_fLookAhead)
		{
			m_fLookAhead = false;

			out = m_chLookAhead;

			return true;
		}
		else
		{
			(*m_pclStream)>>out;

			return m_pclStream->good();
		}
	}

	#define RETURN_TOKEN(X)	do{if(out != NULL) out->assign(m_strToken);m_eTokenType=X;return(X);}while(0);

	ParserTokens_e Parser::GetToken(String_t *out)
	{
		Char_t ch;

		PH_ASSERT_VALID(m_pclStream);
		PH_ASSERT_VALID(this);

		if(m_fTokenAhead)
		{
			m_fTokenAhead = false;

			RETURN_TOKEN(m_eTokenType);
		}

		m_strToken.clear();

		bool e = this->GetNextChar(ch);
		if(e != true)
			RETURN_TOKEN(TOKEN_EOF);

		do
		{
			switch(ch)
			{
				case '\n':
					m_uCurrentLine++;
					break;

				case '\t':
				case '\r':
				case ' ':
					break;

				case '=':
					m_strToken += ch;
					RETURN_TOKEN(TOKEN_EQUAL);
					break;

				case ':':
					m_strToken += ch;
					RETURN_TOKEN(TOKEN_COLON);
					break;

				case ';':
					m_strToken += ch;
					RETURN_TOKEN(TOKEN_SEMI_COLON);
					break;

				case '{':
					m_strToken+=ch;
					RETURN_TOKEN(TOKEN_OPEN_BRACE);
					break;

				case '}':
					m_strToken += ch;
					RETURN_TOKEN(TOKEN_CLOSE_BRACE);
					break;

				case '(':
					m_strToken += ch;
					RETURN_TOKEN(TOKEN_OPEN_PAREN);
					break;

				case ')':
					m_strToken += ch;
					RETURN_TOKEN(TOKEN_CLOSE_PAREN);
					break;
				case '.':
					m_strToken += ch;
					RETURN_TOKEN(TOKEN_DOT);
					break;

				case '/':
					//-------------------------
					(*m_pclStream)>>ch;

					if((!(m_pclStream->good())) || ((ch != '/') && (ch != '*')))
					{
						this->SetLookAhead(ch);
						RETURN_TOKEN(TOKEN_ERROR);
					}

					if(ch == '*')
					{
						bool commentClosing = false;
						for(;;)
						{
							(*m_pclStream)>>ch;
							e = m_pclStream->good();
							if(e != true)
								RETURN_TOKEN(TOKEN_EOF);

							if(ch == '*')
							{
								commentClosing = true;
							}
							else if(commentClosing && ch == '/')
								break;
							else
							{
								commentClosing = false;
								if(ch == '\n')
									++m_uCurrentLine;
							}
						}
					}
					else
					{
						//-----------------------
						do
						{
							(*m_pclStream)>>ch;
							e = m_pclStream->good();
							if(e != true)
								RETURN_TOKEN(TOKEN_EOF);
						} while(ch != '\n');
						m_uCurrentLine++;
					}
					break;

				case '\"':
					do
					{
						(*m_pclStream)>>ch;
						e = m_pclStream->good();
						if(e != true)
							RETURN_TOKEN(TOKEN_ERROR);

						if(ch == '\"')
							RETURN_TOKEN(TOKEN_STRING);

						m_strToken += ch;
					} while(1);
					break;

				default:
					if(IS_NUMBER_START(ch))
					{
						bool gotDot = false;						
						bool hexMode = false;

						m_strToken += ch;

						//Check for hex start sequence
						(*m_pclStream)>>ch;
						if(m_pclStream->good() != true)
						{
							//this is an expected EOF
							RETURN_TOKEN(TOKEN_NUMBER);
						}

						if((ch == 'x') || (ch == 'X'))
						{
							hexMode = true;
							m_strToken += ch;

							(*m_pclStream)>>ch;
							if(m_pclStream->good() != true)
							{
								//this is an expected EOF
								RETURN_TOKEN(TOKEN_NUMBER);
							}
						}						

						while(IS_NUMBER(ch) || (hexMode && IS_HEX_EXTRA_DIGITS(ch)))
						{
							if(ch == '.')
							{
								if(gotDot || hexMode)
									RETURN_TOKEN(TOKEN_ERROR);
								gotDot = true;
							}
							
							m_strToken += ch;							

							(*m_pclStream)>>ch;							

							if(m_pclStream->good() != true)
							{
								//this is an expected EOF
								RETURN_TOKEN(TOKEN_NUMBER);
							}

						} 

						if(hexMode)
						{
							//Convert from hex to decimal
							unsigned x =  std::stoi(m_strToken, 0, 16);												

							m_strToken = std::to_string(x);
						}

						this->SetLookAhead(ch);
						RETURN_TOKEN(TOKEN_NUMBER);
					}
					else if(IS_ID_START(ch))
					{
						do
						{
							m_strToken += ch;

							(*m_pclStream)>>ch;

							e = m_pclStream->good();

							if(e != true)
							{
								//This is a expected EOF
								RETURN_TOKEN(TOKEN_ID);
							}

						} while(IS_ID(ch));

						this->SetLookAhead(ch);
						RETURN_TOKEN(TOKEN_ID);
					}

			}
			(*m_pclStream)>>ch;

			if(m_pclStream->good() != true)
				RETURN_TOKEN(TOKEN_EOF);

		}while(1);

		//shut up compiler
		RETURN_TOKEN(TOKEN_ERROR);
	}


	const Char_t *Parser::GetTokenTypeName(ParserTokens_e token)
	{
		struct TokenName_s
		{
			const Char_t		*pchName;
			ParserTokens_e	eToken;
		};

		static TokenName_s stTokens[] =
		{
			{"number",				TOKEN_NUMBER},
			{"string",				TOKEN_STRING},
			{"id",					TOKEN_ID},
			{"open brace",			TOKEN_OPEN_BRACE},
			{"close brace",			TOKEN_CLOSE_BRACE},
			{"open parenthesis",	TOKEN_OPEN_PAREN},
			{"close parenthesis",	TOKEN_CLOSE_PAREN},
			{"EOF",					TOKEN_EOF},
			{"parse error",			TOKEN_ERROR},
			{NULL,					TOKEN_ERROR}
		};

		UInt_t i = 0;
		for(;stTokens[i].pchName; ++i)
		{
			if(stTokens[i].eToken == token)
				return(stTokens[i].pchName);
		}

		return("INVALID TOKEN");
	}
}
