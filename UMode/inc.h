#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <string_view>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <vector>
#include <charconv>
#include <sstream>
#include <codecvt>
#include <thread>
#include <conio.h> 
#include "Address.h"
#include <stdio.h>
#include <mmsystem.h>
#include "rawdata.h"
#include <map>
#ifdef _RELEASE
#pragma comment(lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment(lib, "curl/libcurl_a.lib")
#endif
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "wldap32.lib")
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32.lib")
#define CURL_STATICLIB
#include "curl/curl.h"
#include "skCrypter.h"
#include "../RebirthGuard/RebirthGuardSDK.h"
#pragma comment (lib, "RebirthGuard.lib")
#pragma comment (lib, "winmm.lib")

using namespace std;

#pragma warning(disable: 4996)