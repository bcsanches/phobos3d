rmdir /s /q c:\temp\release\
mkdir c:\temp\release\resources

copy ..\bin\PH_Kernel.dll c:\temp\release\
copy ..\bin\PH_Input.dll c:\temp\release\
copy ..\bin\PH_GameEngine.dll c:\temp\release\
copy ..\bin\PH_EngineCore.dll c:\temp\release\
copy ..\bin\PH_Dictionary.dll c:\temp\release\
copy ..\bin\PH_MapViewer.dll c:\temp\release\
copy ..\bin\PH_EngineMain.exe c:\temp\release\

copy %OGRE_HOME%\build\bin\release\OgreMain.dll c:\temp\release\
copy %OGRE_HOME%\build\bin\release\RenderSystem_Direct3D9.dll c:\temp\release\
copy %OGRE_HOME%\build\bin\release\Plugin_ParticleFX.dll c:\temp\release\
copy %OGRE_HOME%\build\bin\release\Plugin_CgProgramManager.dll c:\temp\release\
copy %OGRE_HOME%\build\bin\release\cg.dll c:\temp\release\

copy ..\work\mapViewerSample\autoexec.cfg c:\temp\release\
copy ..\work\mapViewerSample\game.cfg c:\temp\release\
xcopy /s ..\work\mapViewerSample\resources\*.* c:\temp\release\resources\
                                          