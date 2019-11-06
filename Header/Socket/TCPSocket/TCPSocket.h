#pragma once

#ifndef __YT_ASIO_SERVER_H_
#define __YT_ASIO_SERVER_H_

#include "../YTSocketThread.h"
#include "../SendBuffer.h"
#include "../MessageQueue.hpp"

namespace YTSvrLib {
	enum EM_ENDPOINT_TYPE {
		EP_NULL,
		EP_CONNECTOR,// ����˵�������
		EP_CLIENT,// �ͻ���(�������ӷ�)
	};

	class ITCPCONTROL;
	/**
	* �����߻���.�������յ�socket����,����/��ȡ
	*/
	class YTSVRLIB_EXPORT ITCPBASE {
	public:
		ITCPBASE();
	public:
		void Send(const char* buf, int len);
		virtual void SafeClose();
		std::string& GetAddrIp();
		int GetAddrPort();
		HANDLE GetSocket();
		BOOL IsSending() const;
		virtual EM_ENDPOINT_TYPE GetEPType() = 0;
	protected:
		void Clean();
		void DoRead();
		void DoSend();
		virtual int OnRecved(const char* msg, int nLen) = 0;
	protected:
		virtual void OnError(int nErrCode);
		void Close();
	protected:
		ITCPCONTROL* m_pSvrMgr;
		std::shared_ptr<asio::ip::tcp::socket> m_Socket;
		std::string m_strDstIP;
		int m_nPort;
		CBuffer<BLOCK_RECV_BUFFER_SIZE> m_recvBuf;
		CSendBuffer m_sendBuf;
		YTSvrLib::CLock m_sendLock;
		BOOL m_bIsSending;
	};

	class ITCPSERVER;
	/*
	* ������Accept��������߻���
	*/
	class YTSVRLIB_EXPORT ITCPCONNECTOR : public ITCPBASE {
	public:
		ITCPCONNECTOR() :ITCPBASE() {}
	public:
		void InitSvr(ITCPSERVER* pMgr, asio::ip::tcp::socket _sock);

		ITCPSERVER* GetMgr() {
			return (ITCPSERVER*)m_pSvrMgr;
		}

		virtual EM_ENDPOINT_TYPE GetEPType() {
			return EP_CONNECTOR;
		}
	};

	class ITCPCLIENTMGR;
	/* �ͻ��˻���
	*/
	class YTSVRLIB_EXPORT ITCPCLIENT : public ITCPBASE {
	public:
		ITCPCLIENT();

		void Connect(ITCPCLIENTMGR* pMgr, const char* ip, int port);

		virtual void SafeClose() override;

		virtual void OnError(int nErrCode) override;

		void OnConnecting(std::error_code ec);

		virtual EM_ENDPOINT_TYPE GetEPType() {
			return EP_CLIENT;
		}

		ITCPCLIENTMGR* GetMgr() {
			return (ITCPCLIENTMGR*)m_pSvrMgr;
		}

		BOOL IsConnecting() const {
			return m_bIsConnecting;
		}

		BOOL IsConnected() const {
			return m_bConnected;
		}
	private:
		CLock m_lockStatus;
		BOOL m_bIsConnecting;
		BOOL m_bConnected;
	};

	/* �����߻���.ͳһ�����̺߳���Ϣ���� */
	class YTSVRLIB_EXPORT ITCPCONTROL : public IASIOTHREAD, public CMessageQueue<ITCPBASE> {};

	/* ������������ */
	class YTSVRLIB_EXPORT ITCPSERVER : public ITCPCONTROL {
	public:
		ITCPSERVER();

		virtual ITCPCONNECTOR* AllocateConnector(std::string dstIP) = 0;

		virtual void ReleaseConnector(YTSvrLib::ITCPCONNECTOR* pConnect) = 0;

		BOOL StartListen(int nPort);
		
		void StopListen();
	private:
		virtual void ProcessDisconnectEvent(ITCPBASE* pConn) override;
	protected:
		void onAccept();
	protected:
		int m_nPort;
		asio::ip::tcp::acceptor m_Acceptor;
	};

	/* �ͻ��˹�����.ͳһ�������пͻ�����һ��io_service֮�� */
	class YTSVRLIB_EXPORT ITCPCLIENTMGR : public ITCPCONTROL {
	public:
		ITCPCLIENTMGR();

		void CreateThread();
	private:
		virtual void ProcessDisconnectEvent(ITCPBASE* pConn) override;
	};
}

#endif