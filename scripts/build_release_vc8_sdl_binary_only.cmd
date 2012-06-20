copy ..\bin\PH_Kernel.dll %1
copy ..\bin\PH_Input.dll %1
copy ..\bin\PH_GameEngine.dll %1
copy ..\bin\PH_EngineCore.dll %1
copy ..\bin\PH_OgreEngineCore.dll %1
copy ..\bin\PH_Dictionary.dll %1
copy ..\bin\PH_MapViewer.dll %1
copy ..\bin\PH_EngineMain.exe %1

copy %OGRE_VC8_BIN%\release\OgreMain.dll %1
copy %OGRE_VC8_BIN%\release\OgreTerrain.dll %1
copy %OGRE_VC8_BIN%\release\OgreRTShaderSystem.dll %1
copy %OGRE_VC8_BIN%\release\OgrePaging.dll %1
copy %OGRE_VC8_BIN%\release\RenderSystem_Direct3D9.dll %1
copy %OGRE_VC8_BIN%\release\Plugin_ParticleFX.dll %1
copy %OGRE_VC8_BIN%\release\Plugin_CgProgramManager.dll %1
copy %OGRE_VC8_BIN%\release\cg.dll %1

copy %SDL_HOME%\VisualC\SDL\Release\SDL.dll %1
