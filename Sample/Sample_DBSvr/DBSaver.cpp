// DBSaver.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <assert.h>
#include "DBSaver.h"

#ifndef LIB_WINDOWS
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#endif // LIB_WINDOWS

// Ψһ��Ӧ�ó������
void DestroyAllObj()
{
	CGMSvrParser::GetInstance()->StopListen();
	CDBCache::GetInstance()->RefreshSQLCache();
	CDBManager::GetInstance()->WaitForAllRequestDone();
}
#ifdef LIB_WINDOWS
BOOL WINAPI ConsoleHandler(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT || dwCtrlType == CTRL_C_EVENT
		|| dwCtrlType == CTRL_BREAK_EVENT)
	{
		LOG("System is Closing...");
		DestroyAllObj();
		LOG("System is Closed...");
		ExitProcess(0);
		//  return FALSE;
	}
	return TRUE;
}
#else
void signal_handle_function(int nSignal)
{
	LOG("signal_handle_function : %d", nSignal);
	DestroyAllObj();
	if (nSignal == SIGSEGV || nSignal == SIGFPE || nSignal == SIGABRT)
	{
		PrintBackTrace();
		signal(nSignal,SIG_DFL);
		return;
	}
	exit(0);
}
#endif // LIB_WINDOWS

extern YTSvrLib::CServerApplication gApp;

#ifdef LIB_WINDOWS
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
#else
int main(int argc, char* argv[])
#endif //
{
#ifdef LIB_WINDOWS
	SetConsoleCtrlHandler(ConsoleHandler, true);
#else
	SetConsoleCtrlHandler(signal_handle_function);
#endif // LIB_WINDOWS

	//ע���¼�
	gApp.RegisterEvent( EAppEvent::eAppGMSvrSocketEvent, CGMSvrParser::OnMsgRecv );
	gApp.RegisterEvent( EAppEvent::eAppGMSvrSocketDisconnectEvent, CGMSvrParser::OnDisconnectMsgRecv );
	gApp.RegisterEvent( EAppEvent::eAppGameDB, CDBManager::OnDataRecv );
	gApp.RegisterEvent(EAppEvent::eAppTimerMgrOnTimer, CTimerMgr::OnTimer );

	//��ȡ����
	CConfig::GetInstance();
	CTimerMgr::GetInstance()->CreateTimer(500);

	//��ʼ���¼�
	if (CConfig::GetInstance()->IsInitSQLExist())
	{// �����Ҫ��ʼ��ִ�д洢.����Ҫ��׼�������ݿ�����.�������ʼ��ִ��.�ٿ�������
		CDBManager::GetInstance()->SetConnection(
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLDB.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLHost.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLUser.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLPass.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_nMySQLPort,
			CConfig::GetInstance()->m_sDBConnectInfo.m_nDBThreads,
			CConfig::GetInstance()->m_nDBRetryCount);
		CDBManager::GetInstance()->Init();
		CConfig::GetInstance()->LoadIniSQL();
		CGMSvrParser::GetInstance()->StartListen(CConfig::GetInstance()->m_nGMSvrListenPort, CConfig::GetInstance()->m_strGMSvrListenIPAddr.c_str());
	}
	else
	{// ��������ȿ��������Լӿ������ٶ�
		CGMSvrParser::GetInstance()->StartListen(CConfig::GetInstance()->m_nGMSvrListenPort, CConfig::GetInstance()->m_strGMSvrListenIPAddr.c_str());
		CDBManager::GetInstance()->SetConnection(
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLDB.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLHost.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLUser.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_strMySQLPass.c_str(),
			CConfig::GetInstance()->m_sDBConnectInfo.m_nMySQLPort,
			CConfig::GetInstance()->m_sDBConnectInfo.m_nDBThreads,
			CConfig::GetInstance()->m_nDBRetryCount);
		CDBManager::GetInstance()->Init();
	}

	gApp.Run();
	LOG("System is closing...");
	CDBManager::GetInstance()->WaitForAllRequestDone();

	return 0;
}