#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#include "CWSDK/cwsdk.h"
#include <fstream>
#include "CCSVParse.h"
#include "kmp.hpp"
#include "MemoryHelper.h"
#include "utfconvert.h"
#include "logger.h"

#define NO_DEBUG_LOG

#ifndef NO_DEBUG_LOG
LOGGER::CLogger* logger;
#endif // NO_DEBUG_LOG

#endif // __MAIN_H__
