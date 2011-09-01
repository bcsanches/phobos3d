TEMPLATE = lib

SOURCES += \
    ../../../src/kernel/PH_Window.cpp \
    ../../../src/kernel/PH_String.cpp \
    ../../../src/kernel/PH_RefCounter.cpp \
    ../../../src/kernel/PH_ProcVector.cpp \
    ../../../src/kernel/PH_Path.cpp \
    ../../../src/kernel/PH_Parser.cpp \
    ../../../src/kernel/PH_ObjectManager.cpp \
    ../../../src/kernel/PH_Object.cpp \
    ../../../src/kernel/PH_Node.cpp \
    ../../../src/kernel/PH_Log.cpp \
    ../../../src/kernel/PH_Kernel.cpp \
    ../../../src/kernel/PH_Folders.cpp \
    ../../../src/kernel/PH_Exception.cpp \
    ../../../src/kernel/PH_EventManager.cpp \
    ../../../src/kernel/PH_DynamicLibrary.cpp \
    ../../../src/kernel/PH_ContextVar.cpp \
    ../../../src/kernel/PH_ContextItem.cpp \
    ../../../src/kernel/PH_ContextCmd.cpp \
    ../../../src/kernel/PH_Context.cpp \
    ../../../src/kernel/SDL/PH_WindowSDL.cpp \
    ../../../src/kernel/SDL/PH_EventManagerSDL.cpp

HEADERS += \
    ../../../include/kernel/PH_Window.h \
    ../../../include/kernel/PH_Types.h \
    ../../../include/kernel/PH_String.h \
    ../../../include/kernel/PH_Singleton.h \
    ../../../include/kernel/PH_RefCounterFwd.h \
    ../../../include/kernel/PH_RefCounter.h \
    ../../../include/kernel/PH_ProcVector.h \
    ../../../include/kernel/PH_PathFwd.h \
    ../../../include/kernel/PH_Path.h \
    ../../../include/kernel/PH_Parser.h \
    ../../../include/kernel/PH_ObjectManager.h \
    ../../../include/kernel/PH_Object.h \
    ../../../include/kernel/PH_NodeProperty.h \
    ../../../include/kernel/PH_NodeFwd.h \
    ../../../include/kernel/PH_Node.h \
    ../../../include/kernel/PH_MemoryFunctions.h \
    ../../../include/kernel/PH_Log.h \
    ../../../include/kernel/PH_Listener.h \
    ../../../include/kernel/PH_KernelAPI.h \
    ../../../include/kernel/PH_Kernel.h \
    ../../../include/kernel/PH_InputActions.h \
    ../../../include/kernel/PH_IContext.h \
    ../../../include/kernel/PH_GenericFactory.h \
    ../../../include/kernel/PH_Folders.h \
    ../../../include/kernel/PH_Exception.h \
    ../../../include/kernel/PH_EventManagerFwd.h \
    ../../../include/kernel/PH_EventManager.h \
    ../../../include/kernel/PH_EventListenerFwd.h \
    ../../../include/kernel/PH_EventListener.h \
    ../../../include/kernel/PH_Error.h \
    ../../../include/kernel/PH_Enum.h \
    ../../../include/kernel/PH_DynamicLibrary.h \
    ../../../include/kernel/PH_Defs.h \
    ../../../include/kernel/PH_ContextVar.h \
    ../../../include/kernel/PH_ContextUtils.h \
    ../../../include/kernel/PH_ContextItem.h \
    ../../../include/kernel/PH_ContextCmd.h \
    ../../../include/kernel/PH_Context.h \
    ../../../include/kernel/SDL/PH_WindowSDL.h \
    ../../../include/kernel/SDL/PH_EventManagerSDL.h

INCLUDEPATH += \
    ../../../include/kernel/

DEFINES += \
    PH_LINUX \
    PH_DEBUG \
    PH_CHECK_ASSERT \
    PH_GCC \
    PH_SDL

unix|win32: LIBS += -lSDL
