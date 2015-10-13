#pragma once

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#define WIN32_LEAN_AND_MEAN            // Windows ヘッダーから使用されていない部分を除外します。

// Windows ヘッダー ファイル:
#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <malloc.h>
#include <tchar.h>

#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "zlib.lib")

#include "png.h"
#include "zlib.h"
