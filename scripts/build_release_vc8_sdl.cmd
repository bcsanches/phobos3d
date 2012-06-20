rmdir /s /q c:\temp\release\
mkdir c:\temp\release\resources
                                         
call build_release_vc8_sdl_binary_only.cmd c:\temp\release\ 

copy ..\work\mapViewerSample\autoexec.cfg c:\temp\release\
copy ..\work\mapViewerSample\game.cfg c:\temp\release\
xcopy /s ..\work\mapViewerSample\resources\*.* c:\temp\release\resources\
                                          