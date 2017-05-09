#pragma once
#include "GWServerSocket.h"
#include "Socket/YTSocketServer.h"

class CGWSvrParser : public YTSvrLib::ITCPSERVER, public YTSvrLib::CPkgParserBase, public YTSvrLib::CSingle<CGWSvrParser>
{
public:
	CGWSvrParser(void);
	virtual ~CGWSvrParser(void);

public:
	virtual void SetEvent();
	virtual void SetDisconnectEvent();

	static void OnMsgRecv()// ��Ϣ�������.��������
	{
		GetInstance()->OnMessageRecv();
	}

	static void OnDisconnectMsgRecv()// �յ��Ͽ����ӵ���Ϣ.��������.
	{
		GetInstance()->OnDisconnectMessage();
	}

	virtual void ProcessMessage(YTSvrLib::ITCPBASE* pSocket, const char *pBuf, int nLen);// ������ͨ��������Ϣ

	virtual void ProcessAcceptedMsg(YTSvrLib::ITCPBASE* pSocket);// ����Accept������Ϣ

	virtual void ProcessDisconnectMsg(YTSvrLib::ITCPBASE* pSocket);// ����Ͽ�������Ϣ

	virtual YTSvrLib::ITCPCONNECTOR* AllocateConnector();// ����һ��������

	virtual void ReleaseConnector(YTSvrLib::ITCPCONNECTOR* pConnect);// ����һ��������

	virtual void OnServerClose(){}// �������������

	void OnGWSvrConnected(CGWSvrSocket* pGWSvrSocket);
	void OnGWSvrDisconnect( CGWSvrSocket* pSocket );

	void SendClientMsg( CGWSvrSocket* pGWSocket , LPCSTR pszMsg, int nLen );
	void SendSvrMsg( CGWSvrSocket* pGWSocket , LPCSTR pszMsg, int nMsgLen);
private:
	YTSvrLib::CPool<CGWSvrSocket,2>  m_poolGateway;
};