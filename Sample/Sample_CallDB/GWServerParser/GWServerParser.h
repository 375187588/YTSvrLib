#pragma once
#include "GWServerSocket.h"
#include "Socket/TCPSocket/TCPSocket.h"

class CGWSvrParser : public YTSvrLib::ITCPSERVER, public YTSvrLib::CSingle<CGWSvrParser>
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

	virtual void ProcessMessage(YTSvrLib::ITCPBASE* pSocket, const char *pBuf, int nLen) override;// ����������Ϣ

	virtual void ProcessEvent(YTSvrLib::EM_MESSAGE_TYPE emType, YTSvrLib::ITCPBASE* pConn) override;// ���������¼�

	virtual YTSvrLib::ITCPCONNECTOR* AllocateConnector(std::string);// ����һ��������

	virtual void ReleaseConnector(YTSvrLib::ITCPCONNECTOR* pConnect);// ����һ��������

	virtual void OnServerClose(){}// �������������

	void OnGWSvrConnected(CGWSvrSocket* pGWSvrSocket);
	void OnGWSvrDisconnect( CGWSvrSocket* pSocket );

	void SendClientMsg( CGWSvrSocket* pGWSocket , LPCSTR pszMsg, int nLen );
	void SendSvrMsg( CGWSvrSocket* pGWSocket , LPCSTR pszMsg, int nMsgLen);
private:
	YTSvrLib::CPool<CGWSvrSocket,2>  m_poolGateway;
};