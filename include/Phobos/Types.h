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
#ifndef PH_TYPES_H
#define PH_TYPES_H

#include "Phobos/Defs.h"

#include <cstring>

namespace Phobos
{

#ifdef PH_WIN32
		typedef unsigned char		UInt8_t;			//u8
		typedef char				SInt8_t;			//s8
		typedef unsigned short		UInt16_t;		//u16
		typedef short				SInt16_t;		//s16
		typedef unsigned int		UInt32_t;		//u32
		typedef int					SInt32_t;		//s32
		typedef __int64				SInt64_t;		//s64
		typedef unsigned __int64	UInt64_t;		//u64
		typedef unsigned int		UInt_t;			//platform dependent type, never store or transmit (u)
  		typedef int					SInt_t;			//platform dependent type, never store or transmit (s)
  		typedef float				Float32_t;		//fp32
  		typedef double				Float64_t;		//pf64
  		typedef Float32_t			Float_t;			//platform dependent type, never store or transmit (f)
		typedef void *				Handler_t;		//platform dependent type, never store or transmit (h)
		typedef bool				Bool_t;			//b
		typedef UInt_t				ErrorHandler_t;	//platform dependent type, never store or transmit (eh)
		typedef SInt8_t				Char_t;			//ch
		typedef size_t				Size_t;			//platform dependent type, never store or transmit (sz)
		typedef UInt32_t			ObjectType_t;

#elif defined PH_LINUX
    
        typedef unsigned char		UInt8_t;			//u8
		typedef char				SInt8_t;			//s8
		typedef unsigned short		UInt16_t;		//u16
		typedef short				SInt16_t;		//s16
		typedef unsigned int		UInt32_t;		//u32
		typedef int					SInt32_t;		//s32
		typedef long long   		SInt64_t;		//s64
		typedef unsigned long long	UInt64_t;		//u64
		typedef unsigned int		UInt_t;			//platform dependent type, never store or transmit (u)
  		typedef int					SInt_t;			//platform dependent type, never store or transmit (s)
  		typedef float				Float32_t;		//fp32
  		typedef double				Float64_t;		//pf64
  		typedef float   			Float_t;			//platform dependent type, never store or transmit (f)
		typedef void *				Handler_t;		//platform dependent type, never store or transmit (h)
		typedef bool				Bool_t;			//b
		typedef UInt_t				ErrorHandler_t;	//platform dependent type, never store or transmit (eh)
		typedef SInt8_t				Char_t;			//ch
		typedef size_t				Size_t;			//platform dependent type, never store or transmit (sz)
		typedef UInt32_t			ObjectType_t;

#else
	#error "Unknown platform"
#endif //WIN32


	template <typename T>
	struct Size_s
	{
		inline Size_s():
			m_tWidth(0),
			m_tHeight(0)
		{
			//empty
		}

		inline Size_s(T width, T height):
			m_tWidth(width),
			m_tHeight(height)
		{
			//empty
		}

		inline Size_s(const Size_s &rhs):
			m_tWidth(rhs.m_tWidth),
			m_tHeight(rhs.m_tHeight)
		{
			//empty
		}

		T m_tWidth;
		T m_tHeight;
	};

	typedef Size_s<UInt_t> UIntSize_t;

	template <typename T>
	struct Rect_s
	{
		inline Rect_s()
		{
            m_tOrigin[0] = m_tOrigin[1] = 0;
		}

		inline Rect_s(T x, T y, const Size_s<T> &size):
			m_tSize(size)
		{
			m_tOrigin[0] = x;
            m_tOrigin[1] = y;
		}

		inline Rect_s(T x, T y, T width, T height):
			m_tSize(width, height)
		{
            m_tOrigin[0] = x;
            m_tOrigin[1] = y;
		}

		inline Rect_s(const Rect_s &rhs):
			m_tSize(rhs.m_tSize)		
		{
            m_tOrigin[0] = rhs.m_tOrigin[0];
            m_tOrigin[1] = rhs.m_tOrigin[1];
		}

        inline bool operator == (const Rect_s &other) const
        {
            return  m_tOrigin[0]        == other.m_tOrigin[0] &&
                    m_tOrigin[1]        == other.m_tOrigin[1] &&
                    m_tSize.m_tWidth    == other.m_tSize.m_tWidth &&
                    m_tSize.m_tHeight   == other.m_tSize.m_tHeight;
                   
        }

        inline bool operator != (const Rect_s &other) const
        {
            return !(*this == other);
        }

		inline Rect_s &operator=(const Rect_s &other)
		{
			m_tOrigin[0] = other.m_tOrigin[0];
			m_tOrigin[1] = other.m_tOrigin[1];

			m_tSize = other.m_tSize;		

			return *this;
		}

		T			m_tOrigin[2];
		Size_s<T>	m_tSize;	
	};
}


#endif //PH_TYPES_H
