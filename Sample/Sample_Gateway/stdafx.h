// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#if (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))
#include "targetver.h"
#pragma execution_character_set("utf-8")
#endif

#include "YTSvrLib.h"

#define USE_CHATSVR
#define USE_ADMINSVR

#include "../Common/GlobalDefine.h"
#include "Global.h"
#include "Config/Config.h"

#include "timer/TimerMgr.h"
#include "ServerParser/ServerParser.h"
#include "ClientParser/PkgParser.h"
#include "ClientParser/MsgRetDef.h"