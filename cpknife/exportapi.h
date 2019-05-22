// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CPKNIFE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CPKNIFE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CPKNIFE_EXPORTS
#define CPKNIFE_API __declspec(dllexport)
#define CPKNIFE_TMP_API __declspec(dllexport)
#else
#define CPKNIFE_API __declspec(dllimport)
#define CPKNIFE_TMP_API
#endif

