// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once



#if (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))
#include "targetver.h"
#pragma execution_character_set("utf-8")
#endif

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