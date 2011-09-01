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
	${OBJECTDIR}/_ext/1687502832/PH_MouseInputDevice.o \
	${OBJECTDIR}/_ext/1687502832/PH_KeyboardInputDevice.o \
	${OBJECTDIR}/_ext/1687502832/PH_Button.o \
	${OBJECTDIR}/_ext/1687502832/PH_InputManager.o \
	${OBJECTDIR}/_ext/1758829188/PH_KeyboardInputDeviceSDL.o \
	${OBJECTDIR}/_ext/1687502832/PH_InputMapper.o \
	${OBJECTDIR}/_ext/1758829188/PH_InputManagerSDL.o \
	${OBJECTDIR}/_ext/1687502832/PH_Thumb.o \
	${OBJECTDIR}/_ext/1687502832/PH_InputDevice.o \
	${OBJECTDIR}/_ext/1758829188/PH_MouseInputDeviceSDL.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/libPH_Input.so

dist/Release/GNU-Linux-x86/libPH_Input.so: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libPH_Input.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1687502832/PH_MouseInputDevice.o: ../../../src/input/PH_MouseInputDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1687502832
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1687502832/PH_MouseInputDevice.o ../../../src/input/PH_MouseInputDevice.cpp

${OBJECTDIR}/_ext/1687502832/PH_KeyboardInputDevice.o: ../../../src/input/PH_KeyboardInputDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1687502832
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1687502832/PH_KeyboardInputDevice.o ../../../src/input/PH_KeyboardInputDevice.cpp

${OBJECTDIR}/_ext/1687502832/PH_Button.o: ../../../src/input/PH_Button.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1687502832
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1687502832/PH_Button.o ../../../src/input/PH_Button.cpp

${OBJECTDIR}/_ext/1687502832/PH_InputManager.o: ../../../src/input/PH_InputManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1687502832
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1687502832/PH_InputManager.o ../../../src/input/PH_InputManager.cpp

${OBJECTDIR}/_ext/1758829188/PH_KeyboardInputDeviceSDL.o: ../../../src/input/SDL/PH_KeyboardInputDeviceSDL.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1758829188
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1758829188/PH_KeyboardInputDeviceSDL.o ../../../src/input/SDL/PH_KeyboardInputDeviceSDL.cpp

${OBJECTDIR}/_ext/1687502832/PH_InputMapper.o: ../../../src/input/PH_InputMapper.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1687502832
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1687502832/PH_InputMapper.o ../../../src/input/PH_InputMapper.cpp

${OBJECTDIR}/_ext/1758829188/PH_InputManagerSDL.o: ../../../src/input/SDL/PH_InputManagerSDL.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1758829188
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1758829188/PH_InputManagerSDL.o ../../../src/input/SDL/PH_InputManagerSDL.cpp

${OBJECTDIR}/_ext/1687502832/PH_Thumb.o: ../../../src/input/PH_Thumb.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1687502832
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1687502832/PH_Thumb.o ../../../src/input/PH_Thumb.cpp

${OBJECTDIR}/_ext/1687502832/PH_InputDevice.o: ../../../src/input/PH_InputDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1687502832
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1687502832/PH_InputDevice.o ../../../src/input/PH_InputDevice.cpp

${OBJECTDIR}/_ext/1758829188/PH_MouseInputDeviceSDL.o: ../../../src/input/SDL/PH_MouseInputDeviceSDL.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1758829188
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1758829188/PH_MouseInputDeviceSDL.o ../../../src/input/SDL/PH_MouseInputDeviceSDL.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/libPH_Input.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
