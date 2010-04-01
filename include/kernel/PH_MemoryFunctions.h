#ifndef PH_MEMORY_FUNCTIONS_H
#define PH_MEMORY_FUNCTIONS_H

#include <stdlib.h>

namespace Phobos
{
	inline void *MemoryCopy(void *dest, const void *source, size_t num)
	{
		return memcpy(dest, source, num);
	}

	inline void *MemorySet(void *ptr, int value, size_t num)
	{
		return memset(ptr, value, num);
	}

	inline void MemoryZero(void *dest, size_t num)
	{
		memset(dest, 0, num);
	}

	inline void *MemoryMove(void *dest, const void *src, size_t num)
	{
		return memmove(dest, src, num);
	}
}

#endif