// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#if (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))
#include "targetver.h"
#include <tchar.h>

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <winsock2.h>
#include <Mswsock.h>
//#include <Windows.h>

#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#pragma comment(lib,"mysql/lib/mysqlpp.lib")
//#endif
#pragma comment(lib, "libevent/libevent_core.lib")
#pragma comment(lib, "BugReport/BugslayerUtil.lib")
#pragma comment(lib, "iconv/lib/iconv.lib")
#pragma comment(lib, "jsoncpp/jsoncpp.lib")
#pragma comment( lib, "Winmm.lib" )
#ifdef _WIN64
#pragma comment(lib, "libcurl/libcurl64.lib")
#else
#pragma comment(lib, "libcurl/libcurl32.lib")
#endif

#ifdef _WIN64
#ifdef DEBUG64
#pragma comment(lib, "YTSvrLib_x64D.lib")
#else
#pragma comment(lib, "YTSvrLib_x64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "YTSvrLib_x86D.lib")
#else
#pragma comment(lib, "YTSvrLib_x86.lib")
#endif
#endif

#endif

//������Ķ���
#include "YTSvrLib.h"
#include "../Common/GlobalDefine.h"

#include "global/Global.h"
#include "Config/Config.h"
#include "GMSvrParser/GMSvrDef.h"
#include "dbcache/DBCache.h"
#include "GMSvrParser/GMSvrParser.h"
#include "db/DBManager.h"
#include "timer/TimerMgr.h"