/******************************************************************************
*
* COPYRIGHT BCSoftware LTDA 2009. ALL RIGHTS RESERVED.
*
* This software cannot be copied, stored, distributed without BCSoftware LTDA
* prior authorization.
*
******************************************************************************/

#ifndef IM_STRING_TEST_H
#define IM_STRING_TEST_H

#include <IM_types.h>

class IM_StringTest_c
{
	public:
		static IM_ErrorHandler_t Run(void);

	private:
		static IM_ErrorHandler_t TestTrim(void);
		static IM_ErrorHandler_t TestIsEmpty(void);

		static IM_ErrorHandler_t TestStrip(void);
};

#endif
