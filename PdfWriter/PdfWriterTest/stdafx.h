// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/PdfWriter.lib")
#pragma comment(lib, "../../SDK/lib/win_64/DEBUG/graphics.lib")
#pragma comment(lib, "../../ASCOfficeUtils/ASCOfficeUtilsLib/Win/x64/Debug/ASCOfficeUtilsLib.lib")
#pragma comment(lib, "Ws2_32.lib")
#else
#pragma comment(lib, "../x64/Release/PdfWriter.lib")
#pragma comment(lib, "../../SDK/lib/win_64/graphics.lib")
#pragma comment(lib, "../../ASCOfficeUtils/ASCOfficeUtilsLib/Win/x64/Release/ASCOfficeUtilsLib.lib")
#pragma comment(lib, "Ws2_32.lib")
#endif



// TODO: reference additional headers your program requires here
