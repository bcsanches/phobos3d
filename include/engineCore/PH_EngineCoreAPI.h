#ifndef PH_ENGINE_CORE_API_H
#define PH_ENGINE_CORE_API_H

#ifdef PH_WIN32
	#ifdef PH_ENGINE_CORE_EXPORTS
		#define PH_ENGINE_CORE_API __declspec(dllexport)
	#else
		#define PH_ENGINE_CORE_API
	#endif

	#define PH_ENGINE_CORE_API_PROC extern "C" PH_ENGINE_CORE_API	
#else
	#error "Platform not defined"
#endif

#endif
