#pragma once
#include "GWServerSocket.h"
#include "Socket/RUDP/UDPSocket.h"

class CGWSvrParser : public YTSvrLib::IUDPSERVER, public YTSvrLib::CSingle<CGWSvrParser>
{
public:
	CGWSvrParser(void);
	virtual ~CGWSvrParser(void);

public:
	virtual void SetEvent();

	static void OnMsgRecv()// ��Ϣ�������.��������
	{
		GetInstance()->MessageConsumer();
	}

	virtual void ProcessMessage(YTSvrLib::IUDPBASE* pSocket, const char *pBuf, int nLen) override;// ������ͨ��������Ϣ

	virtual void ProcessEvent(YTSvrLib::EM_MESSAGE_TYPE emType, YTSvrLib::IUDPBASE* pConn) override;

	virtual YTSvrLib::IUDPCONNECTOR* AllocateConnector(std::string dstIP) override;// ����һ��������

	virtual void ReleaseConnector(YTSvrLib::IUDPCONNECTOR* pConn);// ����һ��������

	void OnGWSvrConnected(CGWSvrSocket* pGWSvrSocket);
	void OnGWSvrDisconnect( CGWSvrSocket* pSocket );

	void SendClientMsg( CGWSvrSocket* pGWSocket , LPCSTR pszMsg, int nLen );
	void SendSvrMsg( CGWSvrSocket* pGWSocket , LPCSTR pszMsg, int nMsgLen);
private:
	YTSvrLib::CPool<CGWSvrSocket,2>  m_poolGateway;
};