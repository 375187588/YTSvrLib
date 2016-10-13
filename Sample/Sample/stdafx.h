// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#if (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))

#pragma execution_character_set("utf-8")

#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <winsock2.h>
#include <Mswsock.h>
#include <process.h>
#include <afxtempl.h>
#include <Shlwapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mysql/lib/mysqlpp.lib")
#pragma comment(lib, "libevent/libevent_core.lib")
#pragma comment(lib, "BugReport/BugslayerUtil.lib")
#pragma comment(lib, "iconv/lib/iconv.lib")
#pragma comment(lib, "jsoncpp/jsoncpp.lib")
#ifdef DEBUG64
#pragma comment(lib, "redis/lib/hiredisd.lib")
#else
#pragma comment(lib, "redis/lib/hiredis.lib")
#endif // DEBUG64

#pragma comment( lib, "Winmm.lib" )

#ifdef _WIN64
#pragma  comment(lib, "zlib/zlibmt_x64.lib")
#else
#pragma  comment(lib, "zlib/zlibmt_x86.lib")
#endif

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
#endif // _WIN64

#endif // (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))

#include <unordered_map>
#include <unordered_set>
//������Ķ���
#include "YTSvrLib.h"

#include "../Common/GlobalDefine.h"

#include "global/ServerDefine.h"
#include "global/MessageDefine.h"
#include "timer/TimerMgr.h"
#include "Config/Config.h"
#include "db/DBManager.h"
#include "GWServerParser/GWServerParser.h"
#include "timer/TimerMgr.h"
#include "db/DBLogMgr.h"