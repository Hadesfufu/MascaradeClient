#ifndef __DEBUG_H_INCLUDED__
#define __DEBUG_H_INCLUDED__

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

//	basic include and define
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//	custom define
#pragma comment(lib,"user32.lib")

//custom include
#include <stdio.h>
#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>

LPWSTR charToLPWSTR(const char* input);
//	custom assert to display file and line abord
//	also display custom message in alertBox
void GfxDbgAssert(bool bExpectedTrue, const char * pMessage = 0);

//	custom printf to get display in consoleoutput
int GfxDbgPrintf(const char * pFormat, ...);

#endif
