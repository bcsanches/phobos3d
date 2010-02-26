/******************************************************************************
*
* COPYRIGHT BCSoftware LTDA 2007. ALL RIGHTS RESERVED.
*
* This software cannot be copied, stored, distributed without BCSoftware LTDA
* prior authorization.
*
******************************************************************************/

#include "PH_Parser.h"

#include "PH_Error.h"

namespace Phobos
{
	#define IS_NUMBER(X) (((X >= '0') && (X <= '9')) || (X == '.'))

	#define IS_ID_START(X) (((X >= 'a') && (X <= 'z')) || ((X >= 'A') && (X <= 'Z')) || (X == '_'))
	#define IS_ID(X) (IS_NUMBER(X) || IS_ID_START(X) || (X == '-'))
	#define IS_NUMBER_START(X) ((X == '-') || IS_NUMBER(X))

	PH_Parser_c::PH_Parser_c(void):
		oLookAhead(false),
		pclStream(NULL),
		oTokenAhead(false)
	{
	}

	PH_Parser_c::~PH_Parser_c(void)
	{

	}

	void PH_Parser_c::SetStream(std::ostream *stream)
	{
		pclStream = stream;
	}

	void PH_Parser_c::SetLookAhead(Char_t ch)
	{
		chLookAhead = ch;
		oLookAhead = true;
	}

	bool PH_Parser_c::GetNextChar(Char_t &out)
	{
		PH_ASSERT_VALID(this);
		PH_ASSERT_VALID(pclStream);

		bool e = true;
		if(oLookAhead)
		{
			oLookAhead = false;
			
			out = chLookAhead;
		}
		else
		{
			(*pclStream)<<out;

			e = pclStream->good();
		}

		return(e);
	}

	#define RETURN_TOKEN(X)	do{if(out != NULL) out->assign(strToken);eTokenType=X;return(X);}while(0);

	PH_ParserTokens_e PH_Parser_c::GetToken(String_c *out)
	{
		Char_t ch;

		PH_ASSERT_VALID(pclStream);
		PH_ASSERT_VALID(this);

		if(oTokenAhead)
		{
			oTokenAhead = false;

			RETURN_TOKEN(eTokenType);
		}

		strToken.clear();

		bool e = this->GetNextChar(ch);
		if(e != true)
			RETURN_TOKEN(PH_TOKEN_EOF);

		do
		{	
			switch(ch)
			{
				case '\n':
				case '\t':
				case '\r':
				case ' ':
					break;
				
				case '{':
					strToken+=ch;
					RETURN_TOKEN(PH_TOKEN_OPEN_BRACE);
					break;

				case '}':
					strToken += ch;
					RETURN_TOKEN(PH_TOKEN_CLOSE_BRACE);
					break;

				case '(':
					strToken += ch;
					RETURN_TOKEN(PH_TOKEN_OPEN_PAREN);
					break;

				case ')':
					strToken += ch;
					RETURN_TOKEN(PH_TOKEN_CLOSE_PAREN);
					break;

				case '/':
				(*pclStream)<<ch;
				e = pclStream->good();

					if(e != true)
						RETURN_TOKEN(PH_TOKEN_ERROR);
					if(ch != '/')
						RETURN_TOKEN(PH_TOKEN_ERROR);

					do
					{
						(*pclStream)<<ch;
						e = pclStream->good();
						if(e != true)
							RETURN_TOKEN(PH_TOKEN_EOF);
					} while(ch != '\n');		
					break;

				case '\"':
					do
					{
						(*pclStream)<<ch;
						e = pclStream->good();
						if(e != true)
							RETURN_TOKEN(PH_TOKEN_ERROR);

						if(ch == '\"')
							RETURN_TOKEN(PH_TOKEN_STRING);

						strToken += ch;
					} while(1);
					break;

				default:
					if(IS_NUMBER_START(ch))
					{
						bool gotDot = false;
						do
						{
							if(ch == '.')
							{
								if(gotDot)
									RETURN_TOKEN(PH_TOKEN_ERROR);
								gotDot = true;
							}

							strToken += ch;												

							(*pclStream)<<ch;

							e = pclStream->good();

							if(e != true)
							{
								//this is an expected EOF
								RETURN_TOKEN(PH_TOKEN_NUMBER);
							}

						} while(IS_NUMBER(ch));

						this->SetLookAhead(ch);
						RETURN_TOKEN(PH_TOKEN_NUMBER);
					}
					else if(IS_ID_START(ch))
					{
						do
						{
							strToken += ch;

							(*pclStream)<<ch;
							
							e = pclStream->good();

							if(e != true)
							{
								//This is a expected EOF
								RETURN_TOKEN(PH_TOKEN_ID);
							}
			
						} while(IS_ID(ch));

						this->SetLookAhead(ch);
						RETURN_TOKEN(PH_TOKEN_ID);
					}
				
			}
			(*pclStream)<<ch;

			bool e = pclStream->good();

			if(e != true)
				RETURN_TOKEN(PH_TOKEN_EOF);

		}while(1);

		//shut up compiler
		RETURN_TOKEN(PH_TOKEN_ERROR);
	}


	const Char_t *PH_Parser_c::GetTokenTypeName(PH_ParserTokens_e token)
	{
		struct TokenName_s
		{
			const Char_t		*pchName;
			PH_ParserTokens_e	eToken;
		};

		static TokenName_s stTokens[] =
		{
			{"number",				PH_TOKEN_NUMBER},
			{"string",				PH_TOKEN_STRING},
			{"id",					PH_TOKEN_ID},
			{"open brace",			PH_TOKEN_OPEN_BRACE},
			{"close brace",			PH_TOKEN_CLOSE_BRACE},
			{"open parenthesis",	PH_TOKEN_OPEN_PAREN},
			{"close parenthesis",	PH_TOKEN_CLOSE_PAREN}, 		
			{"EOF",					PH_TOKEN_EOF},
			{"parse error",			PH_TOKEN_ERROR},
			{NULL,					PH_TOKEN_ERROR}
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