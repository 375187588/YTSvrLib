/*MIT License

Copyright (c) 2016 Archer Xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/
#ifndef __YTSOCKETBASE_H__
#define __YTSOCKETBASE_H__

#include <event.h>
#include <event2/bufferevent.h>
#include "SendBuffer.h"

#define BLOCK_RECV_BUFFER_SIZE (1024*8)
namespace YTSvrLib
{
	class ITCPBASE
	{
	public:
		ITCPBASE()
		{
			Clean();
		}

		virtual ~ITCPBASE()
		{

		}

		virtual void Clean()
		{
			ZeroMemory(m_szHost, sizeof(m_szHost));
			m_nPort = 0;
			m_fd = (SOCKET) 0;
			m_pbufferevent = NULL;
			m_recvBuf.Clear();
			m_sendBuf.Clear();
			m_bIsClosed = FALSE;
			m_bIsSending = FALSE;
		}
	public:
		static void OnRead(bufferevent *bev, void *arg);
		static void OnError(bufferevent *bev, short events, void *arg);
	public:
		virtual void OnEventRecvHandle();// �������¼�����.ѭ�����յ�recvBuf��
		virtual int OnSocketRecv();// ����ʵ�ʽ���
		virtual int OnRecved(const char* msg, int nLen) = 0;// ������ϴ������Ϣ.�����Ѵ���Ĵ�С
	public:
		virtual void Send(const char* info,int len);// �������ݵ�������
		virtual void OnSend();// ����ʵ�ʷ���
	public:
		virtual void OnError(int nErrCode) = 0;// �����������
	public:
		virtual void OnConnected(){};// ����ͻ���������Ϣ
	public:
		virtual void SafeClose() = 0;

		virtual BOOL IsDisconnecting() = 0;
		virtual void OnDisconnecting();
		virtual void PostDisconnectMsg(EType eType) = 0;
	public:
		const char* GetAddrIp()
		{
			return m_szHost;
		}

		void SetAddrIP(const char* pszIP)
		{
			strncpy_s(m_szHost, pszIP, 31);
		}

		int GetAddrPort()
		{
			return m_nPort;
		}

		SOCKET GetSocket()
		{
			return m_fd;
		}
	protected:
		char m_szHost[32];
		int m_nPort;
		SOCKET m_fd;
		bufferevent* m_pbufferevent;// event �������
		CBuffer<BLOCK_RECV_BUFFER_SIZE>   m_recvBuf;
		CSendBuffer m_sendBuf;
		YTSvrLib::CLock m_sendLock;
	protected:
		BOOL m_bIsClosed;
		BOOL m_bIsSending;
	};
}

#endif // !__YTSOCKETBASE_H__
