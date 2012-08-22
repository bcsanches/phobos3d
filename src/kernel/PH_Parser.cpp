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
#include "PH_Parser.h"

#include "PH_Error.h"
#include <iostream>

namespace Phobos
{
	#define IS_DIGIT(X) (((X >= '0') && (X <= '9')))
	#define IS_NUMBER(X) (IS_DIGIT(X) || (X == '.'))	
	#define IS_HEX_EXTRA_DIGITS(X) (((X >= 'a') && (X <= 'f')) || ((X >= 'A') && (X <= 'F')))

	#define IS_ID_START(X) (((X >= 'a') && (X <= 'z')) || ((X >= 'A') && (X <= 'Z')) || (X == '_'))
	#define IS_ID(X) (IS_DIGIT(X) || IS_ID_START(X) || (X == '-'))
	#define IS_NUMBER_START(X) ((X == '-') || IS_NUMBER(X))

	Parser_c::Parser_c(void):
        pclStream(NULL),
		fLookAhead(false),
		fTokenAhead(false)
	{
	}

	Parser_c::~Parser_c(void)
	{

	}

	void Parser_c::SetStream(std::istream *stream)
	{
		pclStream = stream;
		uCurrentLine = 0;

		if(stream != NULL)
			(*stream) >> std::noskipws;
	}

	void Parser_c::SetLookAhead(Char_t ch)
	{
		chLookAhead = ch;
		fLookAhead = true;
	}

	bool Parser_c::GetNextChar(Char_t &out)
	{
		PH_ASSERT_VALID(this);
		PH_ASSERT_VALID(pclStream);

		if(fLookAhead)
		{
			fLookAhead = false;

			out = chLookAhead;

			return true;
		}
		else
		{
			(*pclStream)>>out;

			return pclStream->good();
		}
	}

	#define RETURN_TOKEN(X)	do{if(out != NULL) out->assign(strToken);eTokenType=X;return(X);}while(0);

	ParserTokens_e Parser_c::GetToken(String_c *out)
	{
		Char_t ch;

		PH_ASSERT_VALID(pclStream);
		PH_ASSERT_VALID(this);

		if(fTokenAhead)
		{
			fTokenAhead = false;

			RETURN_TOKEN(eTokenType);
		}

		strToken.clear();

		bool e = this->GetNextChar(ch);
		if(e != true)
			RETURN_TOKEN(TOKEN_EOF);

		do
		{
			switch(ch)
			{
				case '\n':
					uCurrentLine++;
					break;

				case '\t':
				case '\r':
				case ' ':
					break;

				case '=':
					strToken += ch;
					RETURN_TOKEN(TOKEN_EQUAL);
					break;

				case ':':
					strToken += ch;
					RETURN_TOKEN(TOKEN_COLON);
					break;

				case ';':
					strToken += ch;
					RETURN_TOKEN(TOKEN_SEMI_COLON);
					break;

				case '{':
					strToken+=ch;
					RETURN_TOKEN(TOKEN_OPEN_BRACE);
					break;

				case '}':
					strToken += ch;
					RETURN_TOKEN(TOKEN_CLOSE_BRACE);
					break;

				case '(':
					strToken += ch;
					RETURN_TOKEN(TOKEN_OPEN_PAREN);
					break;

				case ')':
					strToken += ch;
					RETURN_TOKEN(TOKEN_CLOSE_PAREN);
					break;
				case '.':
					strToken += ch;
					RETURN_TOKEN(TOKEN_DOT);
					break;

				case '/':
					//-------------------------
					(*pclStream)>>ch;

					if((!(pclStream->good())) || ((ch != '/') && (ch != '*')))
					{
						this->SetLookAhead(ch);
						RETURN_TOKEN(TOKEN_ERROR);
					}

					if(ch == '*')
					{
						bool commentClosing = false;
						for(;;)
						{
							(*pclStream)>>ch;
							e = pclStream->good();
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
									++uCurrentLine;
							}
						}
					}
					else
					{
						//-----------------------
						do
						{
							(*pclStream)>>ch;
							e = pclStream->good();
							if(e != true)
								RETURN_TOKEN(TOKEN_EOF);
						} while(ch != '\n');
						uCurrentLine++;
					}
					break;

				case '\"':
					do
					{
						(*pclStream)>>ch;
						e = pclStream->good();
						if(e != true)
							RETURN_TOKEN(TOKEN_ERROR);

						if(ch == '\"')
							RETURN_TOKEN(TOKEN_STRING);

						strToken += ch;
					} while(1);
					break;

				default:
					if(IS_NUMBER_START(ch))
					{
						bool gotDot = false;						
						bool hexMode = false;

						strToken += ch;

						//Check for hex start sequence
						(*pclStream)>>ch;
						if(pclStream->good() != true)
						{
							//this is an expected EOF
							RETURN_TOKEN(TOKEN_NUMBER);
						}

						if((ch == 'x') || (ch == 'X'))
						{
							hexMode = true;
							strToken += ch;

							(*pclStream)>>ch;
							if(pclStream->good() != true)
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
							
							strToken += ch;							

							(*pclStream)>>ch;							

							if(pclStream->good() != true)
							{
								//this is an expected EOF
								RETURN_TOKEN(TOKEN_NUMBER);
							}

						} 

						if(hexMode)
						{
							std::stringstream stream;
							stream << std::hex << strToken;

							unsigned int x;
							stream >> x;

							strToken = NumberToString(x);
						}

						this->SetLookAhead(ch);
						RETURN_TOKEN(TOKEN_NUMBER);
					}
					else if(IS_ID_START(ch))
					{
						do
						{
							strToken += ch;

							(*pclStream)>>ch;

							e = pclStream->good();

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
			(*pclStream)>>ch;

			if(pclStream->good() != true)
				RETURN_TOKEN(TOKEN_EOF);

		}while(1);

		//shut up compiler
		RETURN_TOKEN(TOKEN_ERROR);
	}


	const Char_t *Parser_c::GetTokenTypeName(ParserTokens_e token)
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
