copy ..\bin\PH_Kernel.dll %1
copy ..\bin\PH_Input.dll %1
copy ..\bin\PH_GameEngine.dll %1
copy ..\bin\PH_EngineCore.dll %1
copy ..\bin\PH_OgreEngineCore.dll %1
copy ..\bin\PH_Dictionary.dll %1
copy ..\bin\PH_MapViewer.dll %1
copy ..\bin\PH_EngineMain.exe %1

copy %OGRE_VC8_HOME%\build\bin\release\OgreMain.dll %1
copy %OGRE_VC8_HOME%\build\bin\release\OgreTerrain.dll %1
copy %OGRE_VC8_HOME%\build\bin\release\OgreRTShaderSystem.dll %1
copy %OGRE_VC8_HOME%\build\bin\release\OgrePaging.dll %1
copy %OGRE_VC8_HOME%\build\bin\release\RenderSystem_Direct3D9.dll %1
copy %OGRE_VC8_HOME%\build\bin\release\Plugin_ParticleFX.dll %1
copy %OGRE_VC8_HOME%\build\bin\release\Plugin_CgProgramManager.dll %1
copy %OGRE_VC8_HOME%\Dependencies\bin\Release\cg.dll %1

copy %LIB_ROCKET_VC8_HOME%\bin\RocketCore.dll %1
copy %LIB_ROCKET_VC8_HOME%\bin\RocketControls.dll %1 

copy %SDL_HOME%\VisualC\SDL\Release\SDL.dll %1
