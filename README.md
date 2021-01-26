# SEF Media Preparer [![Build status](https://ci.appveyor.com/api/projects/status/7iwct8sb2oi8gjrg?svg=true&passingText=Release%20-%20OK&failingText=Release%20-%20Fails&pendingText=Release%20-%20Pending)](https://ci.appveyor.com/project/alecselle/sefmediapreparer) [![Build status](https://ci.appveyor.com/api/projects/status/7iwct8sb2oi8gjrg/branch/Unstable?svg=true&passingText=Unstable%20-%20OK&failingText=Unstable%20-%20Fails&pendingText=Release%20-%20Pending)](https://ci.appveyor.com/project/alecselle/sefmediapreparer/branch/Unstable)
Author: Alec Selle ([*SuperEpicFuntime*](https://superepicfuntime.com), Owner/Lead Developer)<br/><br/>
Graphical program that processes video files to determine whether they match specified formats and can convert them if necessary.<br/>
Currently uses ffmpeg binaries to process files, make sure that ffmpeg.exe and ffprobe.exe are either in the same directory as the executable or are included in your environment's path variable (this will be changed later).

## Environment
Qt version: 5.15.1<br/>
msys2 packages: mingw-w64-x86_64-qt5-static mingw-w64-x86_64-boost mingw-w64-x86_64-rapidjson

## Coming Soon...
1. Use of ffmpeg libraries instead of executables
2. UI Overhaul
