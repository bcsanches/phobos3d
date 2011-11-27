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
	${OBJECTDIR}/_ext/190592110/PH_PointEntity.o \
	${OBJECTDIR}/_ext/190592110/PH_MoverComponent.o \
	${OBJECTDIR}/_ext/190592110/PH_RigidBodyComponent.o \
	${OBJECTDIR}/_ext/190592110/PH_EntityComponentFactory.o \
	${OBJECTDIR}/_ext/190592110/PH_GameCamera.o \
	${OBJECTDIR}/_ext/190592110/PH_LinearMoverComponent.o \
	${OBJECTDIR}/_ext/190592110/PH_BaseClient.o \
	${OBJECTDIR}/_ext/190592110/PH_ModelRendererManager.o \
	${OBJECTDIR}/_ext/190592110/PH_PhysicsManager.o \
	${OBJECTDIR}/_ext/190592110/PH_GameEventManager.o \
	${OBJECTDIR}/_ext/190592110/PH_TransformComponent.o \
	${OBJECTDIR}/_ext/190592110/PH_EntityIO.o \
	${OBJECTDIR}/_ext/190592110/PH_WorldManager.o \
	${OBJECTDIR}/_ext/190592110/PH_EntityComponent.o \
	${OBJECTDIR}/_ext/190592110/PH_SpectatorCameraCommandProducer.o \
	${OBJECTDIR}/_ext/190592110/PH_CollisionMesh.o \
	${OBJECTDIR}/_ext/190592110/PH_MoverManager.o \
	${OBJECTDIR}/_ext/190592110/PH_MapLoader.o \
	${OBJECTDIR}/_ext/190592110/PH_GameEvent.o \
	${OBJECTDIR}/_ext/190592110/PH_EntityEvent.o \
	${OBJECTDIR}/_ext/190592110/PH_SpectatorCamera.o \
	${OBJECTDIR}/_ext/190592110/PH_EntityFactory.o \
	${OBJECTDIR}/_ext/190592110/PH_Entity.o \
	${OBJECTDIR}/_ext/190592110/PH_ModelRendererComponent.o \
	${OBJECTDIR}/_ext/190592110/PH_WorldEntity.o


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
LDLIBSOPTIONS=-L../../../bin -lPH_Kerneld -lPH_Inputd -lPH_EgineCored -lPH_Dictionaryd -lOgreMain -lOgreTerrain -lboost_filesystem -lboost_signals -lboost_system -lBulletCollision -lBulletDynamics

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk ../../../bin/libPH_GameEngine.so

../../../bin/libPH_GameEngine.so: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -shared -o ../../../bin/libPH_GameEngine.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/190592110/PH_PointEntity.o: ../../../src/gameEngine/PH_PointEntity.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_PointEntity.o ../../../src/gameEngine/PH_PointEntity.cpp

${OBJECTDIR}/_ext/190592110/PH_MoverComponent.o: ../../../src/gameEngine/PH_MoverComponent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_MoverComponent.o ../../../src/gameEngine/PH_MoverComponent.cpp

${OBJECTDIR}/_ext/190592110/PH_RigidBodyComponent.o: ../../../src/gameEngine/PH_RigidBodyComponent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_RigidBodyComponent.o ../../../src/gameEngine/PH_RigidBodyComponent.cpp

${OBJECTDIR}/_ext/190592110/PH_EntityComponentFactory.o: ../../../src/gameEngine/PH_EntityComponentFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_EntityComponentFactory.o ../../../src/gameEngine/PH_EntityComponentFactory.cpp

${OBJECTDIR}/_ext/190592110/PH_GameCamera.o: ../../../src/gameEngine/PH_GameCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_GameCamera.o ../../../src/gameEngine/PH_GameCamera.cpp

${OBJECTDIR}/_ext/190592110/PH_LinearMoverComponent.o: ../../../src/gameEngine/PH_LinearMoverComponent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_LinearMoverComponent.o ../../../src/gameEngine/PH_LinearMoverComponent.cpp

${OBJECTDIR}/_ext/190592110/PH_BaseClient.o: ../../../src/gameEngine/PH_BaseClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_BaseClient.o ../../../src/gameEngine/PH_BaseClient.cpp

${OBJECTDIR}/_ext/190592110/PH_ModelRendererManager.o: ../../../src/gameEngine/PH_ModelRendererManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_ModelRendererManager.o ../../../src/gameEngine/PH_ModelRendererManager.cpp

${OBJECTDIR}/_ext/190592110/PH_PhysicsManager.o: ../../../src/gameEngine/PH_PhysicsManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_PhysicsManager.o ../../../src/gameEngine/PH_PhysicsManager.cpp

${OBJECTDIR}/_ext/190592110/PH_GameEventManager.o: ../../../src/gameEngine/PH_GameEventManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_GameEventManager.o ../../../src/gameEngine/PH_GameEventManager.cpp

${OBJECTDIR}/_ext/190592110/PH_TransformComponent.o: ../../../src/gameEngine/PH_TransformComponent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_TransformComponent.o ../../../src/gameEngine/PH_TransformComponent.cpp

${OBJECTDIR}/_ext/190592110/PH_EntityIO.o: ../../../src/gameEngine/PH_EntityIO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_EntityIO.o ../../../src/gameEngine/PH_EntityIO.cpp

${OBJECTDIR}/_ext/190592110/PH_WorldManager.o: ../../../src/gameEngine/PH_WorldManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_WorldManager.o ../../../src/gameEngine/PH_WorldManager.cpp

${OBJECTDIR}/_ext/190592110/PH_EntityComponent.o: ../../../src/gameEngine/PH_EntityComponent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_EntityComponent.o ../../../src/gameEngine/PH_EntityComponent.cpp

${OBJECTDIR}/_ext/190592110/PH_SpectatorCameraCommandProducer.o: ../../../src/gameEngine/PH_SpectatorCameraCommandProducer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_SpectatorCameraCommandProducer.o ../../../src/gameEngine/PH_SpectatorCameraCommandProducer.cpp

${OBJECTDIR}/_ext/190592110/PH_CollisionMesh.o: ../../../src/gameEngine/PH_CollisionMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_CollisionMesh.o ../../../src/gameEngine/PH_CollisionMesh.cpp

${OBJECTDIR}/_ext/190592110/PH_MoverManager.o: ../../../src/gameEngine/PH_MoverManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_MoverManager.o ../../../src/gameEngine/PH_MoverManager.cpp

${OBJECTDIR}/_ext/190592110/PH_MapLoader.o: ../../../src/gameEngine/PH_MapLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_MapLoader.o ../../../src/gameEngine/PH_MapLoader.cpp

${OBJECTDIR}/_ext/190592110/PH_GameEvent.o: ../../../src/gameEngine/PH_GameEvent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_GameEvent.o ../../../src/gameEngine/PH_GameEvent.cpp

${OBJECTDIR}/_ext/190592110/PH_EntityEvent.o: ../../../src/gameEngine/PH_EntityEvent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_EntityEvent.o ../../../src/gameEngine/PH_EntityEvent.cpp

${OBJECTDIR}/_ext/190592110/PH_SpectatorCamera.o: ../../../src/gameEngine/PH_SpectatorCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_SpectatorCamera.o ../../../src/gameEngine/PH_SpectatorCamera.cpp

${OBJECTDIR}/_ext/190592110/PH_EntityFactory.o: ../../../src/gameEngine/PH_EntityFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_EntityFactory.o ../../../src/gameEngine/PH_EntityFactory.cpp

${OBJECTDIR}/_ext/190592110/PH_Entity.o: ../../../src/gameEngine/PH_Entity.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_Entity.o ../../../src/gameEngine/PH_Entity.cpp

${OBJECTDIR}/_ext/190592110/PH_ModelRendererComponent.o: ../../../src/gameEngine/PH_ModelRendererComponent.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_ModelRendererComponent.o ../../../src/gameEngine/PH_ModelRendererComponent.cpp

${OBJECTDIR}/_ext/190592110/PH_WorldEntity.o: ../../../src/gameEngine/PH_WorldEntity.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/190592110
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I../../../include/gameEngine -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/include/bullet -I/home/cronosts/Programming/Libs/rapidxml-1.13 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/190592110/PH_WorldEntity.o ../../../src/gameEngine/PH_WorldEntity.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} ../../../bin/libPH_GameEngine.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
