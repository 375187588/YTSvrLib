// Gateway.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <assert.h>
#include "Gateway.h"
#ifndef LIB_WINDOWS
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#endif // LIB_WINDOWS
extern YTSvrLib::CServerApplication gApp;// Ψһ��Ӧ�ó������

void DestroyAllObj()
{
	CServerParser::GetInstance()->CloseServer();
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
//	fprintf(stderr, "signal_handle_function : %d", nSignal);
	LOG("signal_handle_function : %d", nSignal);
	DestroyAllObj();
	if (nSignal == SIGSEGV || nSignal == SIGFPE || nSignal == SIGABRT)
	{
		fprintf(stderr, "signal_handle_function : %d", nSignal);
		PrintBackTrace();
		signal(nSignal, SIG_DFL);
		return;
	}
	exit(0);
}
#endif // LIB_WINDOWS

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
	gApp.RegisterEvent( EAppEvent::eAppClientSocketEvent, CPkgParser::OnMsgRecv );
	gApp.RegisterEvent( EAppEvent::eAppClientSocketDisconnectEvent, CPkgParser::OnDisconnectMsgRecv );
	gApp.RegisterEvent( EAppEvent::eAppServerSocketEvent, CServerParser::OnMsgRecv );
	gApp.RegisterEvent( EAppEvent::eAppServerSocketDisconnectEvent, CServerParser::OnDisconnectMsgRecv );
	gApp.RegisterEvent(EAppEvent::eAppTimerMgrOnTimer, CTimerMgr::OnTimer );

	//��ȡ����
	CConfig::GetInstance();

	if (argc > 1)
	{
#ifdef LIB_WINDOWS
		UINT nOffset = _tstoi(argv[1]);
#else
		UINT nOffset = atoi(argv[1]);
#endif // LIB_WINDOWS

		CConfig::GetInstance()->SetLocalIDOffset(nOffset);
	}

	//����˿�
	CPkgParser::GetInstance()->StartListen( CConfig::GetInstance()->m_nClientListenPort, CConfig::GetInstance()->m_strClientListenIPAddr.c_str() );
	CServerParser::GetInstance()->InitSvrSocket();

	CTimerMgr::GetInstance()->CreateTimer(500);

	char szTitle[127]={0};
	_snprintf_s( szTitle, 127, "GatewaySvr=P[%d] L[%d] Online Client=%d ...",CConfig::GetInstance()->m_nPublicSvrID,CConfig::GetInstance()->m_nLocalSvrID,CPkgParser::GetInstance()->GetCurClientCount() );
	SetConsoleTitleA( szTitle );

	gApp.Run();

	LOG("System is closed.");
	Sleep(3000);

 	CTimerMgr::DelInstance();
 	CConfig::DelInstance();

	DelLogManager();
	return 0;
}