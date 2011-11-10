#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/821979351/PH_ObjectManager.o \
	${OBJECTDIR}/_ext/821979351/PH_String.o \
	${OBJECTDIR}/_ext/821979351/PH_DynamicLibrary.o \
	${OBJECTDIR}/_ext/821979351/PH_ContextVar.o \
	${OBJECTDIR}/_ext/821979351/PH_EventManager.o \
	${OBJECTDIR}/_ext/821979351/PH_Kernel.o \
	${OBJECTDIR}/_ext/1198965475/PH_WindowSDL.o \
	${OBJECTDIR}/_ext/821979351/PH_RefCounter.o \
	${OBJECTDIR}/_ext/821979351/PH_Exception.o \
	${OBJECTDIR}/_ext/821979351/PH_ContextItem.o \
	${OBJECTDIR}/_ext/821979351/PH_Folders.o \
	${OBJECTDIR}/_ext/821979351/PH_Window.o \
	${OBJECTDIR}/_ext/821979351/PH_Node.o \
	${OBJECTDIR}/_ext/821979351/PH_Path.o \
	${OBJECTDIR}/_ext/821979351/PH_Context.o \
	${OBJECTDIR}/_ext/821979351/PH_Object.o \
	${OBJECTDIR}/_ext/821979351/PH_ContextCmd.o \
	${OBJECTDIR}/_ext/821979351/PH_ProcVector.o \
	${OBJECTDIR}/_ext/1198965475/PH_EventManagerSDL.o \
	${OBJECTDIR}/_ext/821979351/PH_Parser.o \
	${OBJECTDIR}/_ext/821979351/PH_Log.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lSDL

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk ../../../bin/libPH_Kernel.so

../../../bin/libPH_Kernel.so: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -shared -o ../../../bin/libPH_Kernel.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/821979351/PH_ObjectManager.o: ../../../src/kernel/PH_ObjectManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_ObjectManager.o ../../../src/kernel/PH_ObjectManager.cpp

${OBJECTDIR}/_ext/821979351/PH_String.o: ../../../src/kernel/PH_String.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_String.o ../../../src/kernel/PH_String.cpp

${OBJECTDIR}/_ext/821979351/PH_DynamicLibrary.o: ../../../src/kernel/PH_DynamicLibrary.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_DynamicLibrary.o ../../../src/kernel/PH_DynamicLibrary.cpp

${OBJECTDIR}/_ext/821979351/PH_ContextVar.o: ../../../src/kernel/PH_ContextVar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_ContextVar.o ../../../src/kernel/PH_ContextVar.cpp

${OBJECTDIR}/_ext/821979351/PH_EventManager.o: ../../../src/kernel/PH_EventManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_EventManager.o ../../../src/kernel/PH_EventManager.cpp

${OBJECTDIR}/_ext/821979351/PH_Kernel.o: ../../../src/kernel/PH_Kernel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Kernel.o ../../../src/kernel/PH_Kernel.cpp

${OBJECTDIR}/_ext/1198965475/PH_WindowSDL.o: ../../../src/kernel/SDL/PH_WindowSDL.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1198965475
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1198965475/PH_WindowSDL.o ../../../src/kernel/SDL/PH_WindowSDL.cpp

${OBJECTDIR}/_ext/821979351/PH_RefCounter.o: ../../../src/kernel/PH_RefCounter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_RefCounter.o ../../../src/kernel/PH_RefCounter.cpp

${OBJECTDIR}/_ext/821979351/PH_Exception.o: ../../../src/kernel/PH_Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Exception.o ../../../src/kernel/PH_Exception.cpp

${OBJECTDIR}/_ext/821979351/PH_ContextItem.o: ../../../src/kernel/PH_ContextItem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_ContextItem.o ../../../src/kernel/PH_ContextItem.cpp

${OBJECTDIR}/_ext/821979351/PH_Folders.o: ../../../src/kernel/PH_Folders.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Folders.o ../../../src/kernel/PH_Folders.cpp

${OBJECTDIR}/_ext/821979351/PH_Window.o: ../../../src/kernel/PH_Window.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Window.o ../../../src/kernel/PH_Window.cpp

${OBJECTDIR}/_ext/821979351/PH_Node.o: ../../../src/kernel/PH_Node.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Node.o ../../../src/kernel/PH_Node.cpp

${OBJECTDIR}/_ext/821979351/PH_Path.o: ../../../src/kernel/PH_Path.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Path.o ../../../src/kernel/PH_Path.cpp

${OBJECTDIR}/_ext/821979351/PH_Context.o: ../../../src/kernel/PH_Context.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Context.o ../../../src/kernel/PH_Context.cpp

${OBJECTDIR}/_ext/821979351/PH_Object.o: ../../../src/kernel/PH_Object.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Object.o ../../../src/kernel/PH_Object.cpp

${OBJECTDIR}/_ext/821979351/PH_ContextCmd.o: ../../../src/kernel/PH_ContextCmd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_ContextCmd.o ../../../src/kernel/PH_ContextCmd.cpp

${OBJECTDIR}/_ext/821979351/PH_ProcVector.o: ../../../src/kernel/PH_ProcVector.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_ProcVector.o ../../../src/kernel/PH_ProcVector.cpp

${OBJECTDIR}/_ext/1198965475/PH_EventManagerSDL.o: ../../../src/kernel/SDL/PH_EventManagerSDL.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1198965475
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1198965475/PH_EventManagerSDL.o ../../../src/kernel/SDL/PH_EventManagerSDL.cpp

${OBJECTDIR}/_ext/821979351/PH_Parser.o: ../../../src/kernel/PH_Parser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Parser.o ../../../src/kernel/PH_Parser.cpp

${OBJECTDIR}/_ext/821979351/PH_Log.o: ../../../src/kernel/PH_Log.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/821979351
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/821979351/PH_Log.o ../../../src/kernel/PH_Log.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} ../../../bin/libPH_Kernel.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
