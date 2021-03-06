#ifndef PRODUCT_INFO
#define PRODUCT_INFO

#define xstr(s) str(s)
#define str(s) #s

#define PUBLISHER_NAME "SuperEpicFuntime"
#define PUBLISHER_COPYRIGHT "2021"

#define PRODUCT_VERSION xstr(VER_MAJ) "." xstr(VER_MIN) "." xstr(VER_PAT)
#define PRODUCT_NAME "SEFMediaPreparer " PRODUCT_VERSION
#define PRODUCT_EXECUTABLE "SEFMediaPreparer.exe"
#define PRODUCT_LOGO "../seflogo.ico"
#define PRODUCT_DESCRIPTION "SuperEpicFuntime Media Preparer"

#endif // PRODUCT_INFO
