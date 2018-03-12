# SEF Media Preparer
Author: Alec Selle ([*SuperEpicFuntime*](https://superepicfuntime.com), Owner/Lead Developer)<br/>
Graphical program that processes video files to determine whether they match specified formats and can convert them if necessary. Currently uses ffmpeg binaries to process files, make sure that ffmpeg.exe and ffprobe.exe are either in the same directory as the executable or is included in your environment's path variable (this will be changed later).


## Current Release: [1.0.0-alpha](https://github.com/alecselle/sefmediapreparer/releases)
*Files tagged with 'NF' do not include ffmpeg.*<br/>
**Initial Release. Basic features are working.**

## Branches [![Build Status](http://dev.alecselle.com:8080/job/SEF%20Media%20Preparer/job/SEF%20Media%20Preparer%20(Development)/badge/icon)](http://dev.alecselle.com:8080/job/SEF%20Media%20Preparer/job/SEF%20Media%20Preparer%20(Development)/)
[master](https://github.com/alecselle/sefmediapreparer/tree/master) - Current Release
[development](https://github.com/alecselle/sefmediapreparer/tree/development) - *Live* Development

## Build Instructions
Requirements: *(Most can be filled by downloading [Qt](https://www.qt.io/download) and selecting MSVC packages)*
    1. [Visual Studio Build Tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)
	2. [qmake](https://www.qt.io/download)
	3. [jom](https://wiki.qt.io/Jom)

Building: *(Import into Qt Creator to simplify procedure)*
    1. > qmake.exe <Source Directory> -spec win32-msvc
	2. > jom.exe qmake_all

## Coming Soon...
1. Use of ffmpeg libraries instead of executables
2. Static Version that doesn't require dlls or executables
