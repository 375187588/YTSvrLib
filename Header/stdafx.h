// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
#include <crtdbg.h>
#include <cstdarg>
#include "targetver.h"
// #include <afxwin.h>
// #include <afxtempl.h>
#endif // LIB_WINDOWS

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
//#include <afxwin.h>
using namespace std;
#include "YTSvrLib.h"

// #ifdef _WIN64
// #pragma comment(lib, "WS2_x64.Lib")
// #else
#ifdef LIB_WINDOWS
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#endif // LIB_WINDOWS

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




//#endif


