# SEF Media Preparer
Author: Alec Selle ([*SuperEpicFuntime*](https://superepicfuntime.com), Owner/Lead Developer)<br/><br/>
Graphical program that processes video files to determine whether they match specified formats and can convert them if necessary.<br/>
Currently uses ffmpeg binaries to process files, make sure that ffmpeg.exe and ffprobe.exe are either in the same directory as the executable or are included in your environment's path variable (this will be changed later).


## Current Release: [1.1.0-alpha](https://github.com/alecselle/sefmediapreparer/releases)
*Files tagged with 'NF' do not include ffmpeg.*<br/>
**Initial Release. Basic features are working.**

## Branches
[master](https://github.com/alecselle/sefmediapreparer/tree/master)<br/>
- [![](http://dev.alecselle.com:8080/job/SEF%20Media%20Preparer/job/SEF%20Media%20Preparer%20(Release)/badge/icon)](http://dev.alecselle.com:8080/job/SEF%20Media%20Preparer/job/SEF%20Media%20Preparer%20(Release)/)<br/>
- Current Release<br/><br/>
[development](https://github.com/alecselle/sefmediapreparer/tree/development)<br/>
- [![](http://dev.alecselle.com:8080/job/SEF%20Media%20Preparer/job/SEF%20Media%20Preparer%20(Development)/badge/icon)](http://dev.alecselle.com:8080/job/SEF%20Media%20Preparer/job/SEF%20Media%20Preparer%20(Development)/)<br/>
- **Live** Development <br/><br/>

## Build Instructions
### Requirements
*(Most can be filled by downloading [Qt](https://www.qt.io/download) and selecting MSVC packages)*
1. [Visual Studio Build Tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)<br/>
2. [qmake](https://www.qt.io/download)<br/>
3. [jom](https://wiki.qt.io/Jom)<br/>
### Building
`build.bat [-vcvarsall <path to vcvarsall.bat>] [-qmake <path to qmake>] [-jom <path to jom>]`
<br/>or<br/>
```
vcvarsall.bat amd64 *(Setup Environment)*
qmake.exe <project file (.pro)> -spec win32-msvc *(Create Makefile)*
jom.exe *(Build Project)*
```

## Coming Soon...
1. Use of ffmpeg libraries instead of executables
2. Static Version that doesn't require dlls or executables
