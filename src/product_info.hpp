#ifndef PRODUCT_INFO
#define PRODUCT_INFO

#define xstr(s) str(s)
#define str(s) #s

#define PUBLISHER_NAME "SuperEpicFuntime"
#define PUBLISHER_COPYRIGHT "2018"

#define PRODUCT_NAME "SEFMediaPreparer"
#define PRODUCT_EXECUTABLE "SEFMediaPreparer.exe"
#define PRODUCT_LOGO "F:/Workspace/Cpp/SEFMediaPreparer/seflogo.ico"
#define PRODUCT_DESCRIPTION "SuperEpicFuntime Media Preparer"
#define PRODUCT_VERSION xstr(VER_MAJ) "." xstr(VER_MIN) "." xstr(VER_PAT)

#define PRODUCT_INSTALL_PATH "%APPDATA%/SuperEpicFuntime/SEFMediaPreparer"

#endif // PRODUCT_INFO
