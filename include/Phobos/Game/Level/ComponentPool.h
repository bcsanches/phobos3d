
#ifndef PH_GAME_COMPONENT_POOL_H
#define PH_GAME_COMPONENT_POOL_H

#define PH_GAME_DEFINE_COMPONENT_POOL(TYPE, POOL_NAME)	\
	namespace											\
	{													\
		static boost::object_pool<TYPE> POOL_NAME;		\
	}													\
														\
	void *TYPE::operator new(size_t size)				\
	{													\
		PH_ASSERT(size == sizeof(TYPE));				\
														\
		return POOL_NAME.malloc();						\
	}													\
														\
	void TYPE::operator delete(void *ptr)				\
	{													\
		auto *p = static_cast<TYPE *>(ptr);				\
														\
		if (!p)											\
			return;										\
														\
		PH_ASSERT(POOL_NAME.is_from(p));				\
														\
		POOL_NAME.free(p);								\
	}

#endif