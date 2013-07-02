copy ..\bin\PH_Base.dll %1
copy ..\bin\PH_System.dll %1
copy ..\bin\PH_Game.dll %1
copy ..\bin\PH_Engine.dll %1
copy ..\bin\PH_OgreEngine.dll %1
copy ..\bin\PH_Register.dll %1
copy ..\bin\PH_MapViewer.dll %1
copy ..\bin\PH_EngineMain.exe %1

copy %OGRE_HOME%\build\sdk\bin\release\OgreMain.dll %1
copy %OGRE_HOME%\build\sdk\bin\release\OgreTerrain.dll %1
copy %OGRE_HOME%\build\sdk\bin\release\OgreRTShaderSystem.dll %1
copy %OGRE_HOME%\build\sdk\bin\release\OgrePaging.dll %1
copy %OGRE_HOME%\build\sdk\bin\release\OgreOverlay.dll %1
copy %OGRE_HOME%\build\sdk\bin\release\RenderSystem_Direct3D9.dll %1
copy %OGRE_HOME%\build\sdk\bin\release\Plugin_ParticleFX.dll %1
copy %OGRE_HOME%\build\sdk\bin\release\Plugin_CgProgramManager.dll %1
copy %OGRE_DEPS_HOME%\build\bin\Release\cg.dll %1

copy %LIB_ROCKET_HOME%\bin\RocketCore.dll %1
copy %LIB_ROCKET_HOME%\bin\RocketControls.dll %1 

copy ..\contrib\bin\SDL.dll %1
