rmdir /s /q c:\temp\release\
mkdir c:\temp\release\resources

copy ..\bin\PH_Kernel.dll c:\temp\release\
copy ..\bin\PH_Input.dll c:\temp\release\
copy ..\bin\PH_GameEngine.dll c:\temp\release\
copy ..\bin\PH_EngineCore.dll c:\temp\release\
copy ..\bin\PH_Dictionary.dll c:\temp\release\
copy ..\bin\PH_MapViewer.dll c:\temp\release\
copy ..\bin\PH_EngineMain.exe c:\temp\release\

copy %OGRE_VC8_BIN%\release\OgreMain.dll c:\temp\release\
copy %OGRE_VC8_BIN%\release\RenderSystem_Direct3D9.dll c:\temp\release\
copy %OGRE_VC8_BIN%\release\Plugin_ParticleFX.dll c:\temp\release\
copy %OGRE_VC8_BIN%\release\Plugin_CgProgramManager.dll c:\temp\release\
copy %OGRE_VC8_BIN%\release\cg.dll c:\temp\release\

copy ..\work\engineMain\autoexec.cfg c:\temp\release\
copy ..\work\engineMain\mapViewer.cfg c:\temp\release\
xcopy /s ..\work\engineMain\resources\*.* c:\temp\release\resources\
                                          