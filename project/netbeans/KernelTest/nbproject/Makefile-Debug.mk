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
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1200086537/main.o \
	${OBJECTDIR}/_ext/1200086537/ContextVarTest.o \
	${OBJECTDIR}/_ext/1200086537/PathTest.o \
	${OBJECTDIR}/_ext/1200086537/ContextItemTest.o \
	${OBJECTDIR}/_ext/1200086537/ContextCmdTest.o \
	${OBJECTDIR}/_ext/1200086537/StringTest.o \
	${OBJECTDIR}/_ext/1200086537/ParserTest.o \
	${OBJECTDIR}/_ext/1200086537/LogTest.o \
	${OBJECTDIR}/_ext/1200086537/ObjectManagerTest.o \
	${OBJECTDIR}/_ext/1200086537/ContextTest.o \
	${OBJECTDIR}/_ext/1200086537/NodeTest.o \
	${OBJECTDIR}/_ext/1200086537/KernelTest.o \
	${OBJECTDIR}/_ext/1200086537/EventManagerTest.o


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
LDLIBSOPTIONS=-L../../../bin -lboost_system -lboost_filesystem -lboost_unit_test_framework -lPH_Kerneld

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/kerneltest

dist/Debug/GNU-Linux-x86/kerneltest: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kerneltest ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1200086537/main.o: ../../../src/kernelTest/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/main.o ../../../src/kernelTest/main.cpp

${OBJECTDIR}/_ext/1200086537/ContextVarTest.o: ../../../src/kernelTest/ContextVarTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/ContextVarTest.o ../../../src/kernelTest/ContextVarTest.cpp

${OBJECTDIR}/_ext/1200086537/PathTest.o: ../../../src/kernelTest/PathTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/PathTest.o ../../../src/kernelTest/PathTest.cpp

${OBJECTDIR}/_ext/1200086537/ContextItemTest.o: ../../../src/kernelTest/ContextItemTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/ContextItemTest.o ../../../src/kernelTest/ContextItemTest.cpp

${OBJECTDIR}/_ext/1200086537/ContextCmdTest.o: ../../../src/kernelTest/ContextCmdTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/ContextCmdTest.o ../../../src/kernelTest/ContextCmdTest.cpp

${OBJECTDIR}/_ext/1200086537/StringTest.o: ../../../src/kernelTest/StringTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/StringTest.o ../../../src/kernelTest/StringTest.cpp

${OBJECTDIR}/_ext/1200086537/ParserTest.o: ../../../src/kernelTest/ParserTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/ParserTest.o ../../../src/kernelTest/ParserTest.cpp

${OBJECTDIR}/_ext/1200086537/LogTest.o: ../../../src/kernelTest/LogTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/LogTest.o ../../../src/kernelTest/LogTest.cpp

${OBJECTDIR}/_ext/1200086537/ObjectManagerTest.o: ../../../src/kernelTest/ObjectManagerTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/ObjectManagerTest.o ../../../src/kernelTest/ObjectManagerTest.cpp

${OBJECTDIR}/_ext/1200086537/ContextTest.o: ../../../src/kernelTest/ContextTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/ContextTest.o ../../../src/kernelTest/ContextTest.cpp

${OBJECTDIR}/_ext/1200086537/NodeTest.o: ../../../src/kernelTest/NodeTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/NodeTest.o ../../../src/kernelTest/NodeTest.cpp

${OBJECTDIR}/_ext/1200086537/KernelTest.o: ../../../src/kernelTest/KernelTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/KernelTest.o ../../../src/kernelTest/KernelTest.cpp

${OBJECTDIR}/_ext/1200086537/EventManagerTest.o: ../../../src/kernelTest/EventManagerTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1200086537
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/kernelTest -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1200086537/EventManagerTest.o ../../../src/kernelTest/EventManagerTest.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/kerneltest

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
