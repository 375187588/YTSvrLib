#pragma once

// #ifdef LIB_WINDOWS
// 
// #include <windows.h>
// #include "SendBuffer.h"
// 
// #ifndef USE_ACCEPTEX
// #define USE_ACCEPTEX
// #endif
// 
// #ifndef _WIN64
// 
// #define __FreeSocket(nSocket) \
//     {\
//     SOCKET __ASocket=::InterlockedExchange((LONG *)&nSocket,LONG(INVALID_SOCKET));\
//     if (__ASocket!=INVALID_SOCKET)\
//         {\
//         ::shutdown(__ASocket,SD_SEND);\
//         ::closesocket(__ASocket);\
//         }\
//     }\
// 
// #else
// 
// #define __FreeSocket(nSocket) \
// 	{\
// 	SOCKET __ASocket=::InterlockedExchange64((LONG64 *)&nSocket,(LONG64)(INVALID_SOCKET));\
// 	if (__ASocket!=INVALID_SOCKET)\
// 		{\
// 		::shutdown(__ASocket,SD_SEND);\
// 		::closesocket(__ASocket);\
// 		}\
// 	}\
// 
// #endif
// 
// /*
// template<size_t size>
// class CBuffer
// {
// public:
//     //CBuffer(const CBuffer& rSrc){  }
//     void operator=(const CBuffer& rSrc);
//     CBuffer() : m_nLength(0) {}
//     ~CBuffer() {}
// 
//     ///��ȡ��������������
//     size_t GetCapacity() { return size; }
// 
//     ///��ȡ��ǰ�����������������ݵĳ���
//     int GetLength() { return (int)m_nLength; }
// 
//     char * GetIdleBuffer(){ return m_buf+m_nLength;}
// 
//     size_t GetIdleLength(){ return size-m_nLength;}
// 
//     ///�򻺳���������һ�����ݣ������Ӻ󻺳�����Խ�磬�������Ӳ�����ʧ��
//     BOOL AddBuffer(const char* buf, size_t nSize)
//     {
//         if (m_nLength + nSize > size)
//             return FALSE;
//         memcpy((unsigned char*)&m_buf[m_nLength], buf, nSize);
//         m_nLength += nSize;
//         return TRUE;
//     }
// 
//     //���ӻ����������ݵĳ���
//     BOOL AddBuffer(size_t nSize)
//     {
//         if (m_nLength + nSize > size)
//         {
//             return FALSE;
//         }
//         m_nLength += nSize;
//         return TRUE;
//     }
// 
//     ///��ȡ������ͷ�ĵ�ַ
//     char* GetBuffer() { return m_buf; }
// 
//     ///�ͷŴӻ�������ʼ��ָ�����ȵ����ݿռ�
//     void ReleaseBuffer(size_t nLength)
//     {
//         if ( nLength >= m_nLength )
//         {
//             m_nLength = 0;
//         }
//         else
//         {
//             if ( nLength>0 )
//             {
//                 m_nLength -= nLength;
//                 memcpy(&m_buf[0], &m_buf[nLength], m_nLength);
//             }
//             else
//             {
//                 // ����û�û�д�����κ����ݣ�ͬʱ�������Ѿ����ˣ���ջ���
//                 if ( m_nLength>=size )
//                 {
//                     m_nLength = 0;
//                 }
//             }
//         }
//     }
// 
//     ///�ָ���ʼ״̬
//     void Clear()
//     {
//         m_nLength = 0;
//     }
// private:
//     size_t m_nLength;
//     char m_buf[size];
// };
// */
//CBuffer Rewrited by waj, to add function to ReSize

#include "event.h"
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>
#include "SendBuffer.h"

// 
// /**
// @brief ��ʼ��ws2_32.dll
// *
// *	ȫ�ֳ�ʼ����
// */

// 
// 
// /**
// @brief Socketͨ�Žӿڶ���
// *
// *	Socketͨ�Žӿڶ���
// */
// class ISocketBase:public CRecycle
// {
// private:
// 
//     static const INT_PTR ACCEPTEX_BUFFER_SIZE = 256;
// 
// public:
// 
//     enum  ESendType
//     {
//         eSendImmediately,
//         eSendHold
//     };
// 
//     enum IOType
//     {
//         IORecv,
//         IOSend,
//         IOAccept,
//         IOConnect,
//         IODisconnect,
//         IOCount
//     };    
// 
//     //virtual void Init(){};
//     virtual void Clear(){ CloseSocket(); }
// 
//     typedef struct __OverLappedBase:public OVERLAPPED
//     {
//         SOCKET m_hSocket;
//         void    SetType( IOType eType ){ Pointer = (PVOID)eType; }
//         IOType  GetType() { return (IOType)(int)Pointer; }
// 
//         void	SetSocket( SOCKET hSocket )
//         { 
//             m_hSocket = hSocket;
//         }
//         SOCKET	GetSocket()const { return m_hSocket; }
//     }OVERLAPPEDBASE;
//     
//     typedef  OVERLAPPEDBASE DISCONNECTOVERLAP;
// 
//     typedef struct OverlappedPlus:public OVERLAPPEDBASE
//     {
//         WSABUF				m_wsaBuffer;
//     }OVERLAPPEDPLUS,*POVERLAPPEDPLUS;
// 
//     typedef struct AcceptOverPlus:OVERLAPPEDBASE
//     {        
//         char                m_netBuffer[ACCEPTEX_BUFFER_SIZE];
//     }ACCEPTOVERPLUS, * PACCEPTOVERPLUS;
// 
//     typedef OVERLAPPEDBASE CONNECTOVERPLUS, * PCONNECTOVERPLUS;    
// 
// public:    
//     //-----------------------------------------------------------------------------------------------------------
//     virtual bool    StartListen(int nPort, const char* pstrIp);
//     virtual bool    StopListen();
//     //-----------------------------------------------------------------------------------------------------------
//     virtual bool    Accept();
//     virtual bool    OnAccepted( OVERLAPPED* pAcceptedOl, DWORD dwIOSize );
//     void            OnAcceptedRecvSuccess(const char* pBuf, DWORD dwIOSize);
//     //˭���䣬˭ʵ�ַ�����ͷ�
//     virtual ISocketBase*    ApplyNewAcceptedSocket( const char* /*pszRemoteIP*/, USHORT /*nPort*/ ){ return 0; }
//     virtual void    ReclaimObj( ISocketBase* /*pSocket*/ ){}
//     //-----------------------------------------------------------------------------------------------------------
//     void            CloseSocket();
//     //bool            DoFree(){ return ::InterlockedExchange((LONG *)&m_bFree,true) == FALSE; };
//     //void            SetNoFree(){::InterlockedExchange((LONG *)&m_bFree,FALSE);}
//     //-----------------------------------------------------------------------------------------------------------
//     virtual void    SafeClose();
//     virtual bool    Disconnect() ;//��Ҫ���ⲿ���ô˺�������SafeClose����
//     ///���Ӷ˿��ǵ���
//     virtual void    OnDisconnect() ; //�˺������÷��������ӹر��Ժ�,�����ǿͻ������ӹر�,Ҳ�����ǽ��յ��׽��ֹر�
//     DISCONNECTOVERLAP* GetDisconnectOL(){ return &m_olDisConnect; }
//     void            OnDisconnectEvent();
//     virtual void    PostDisconnectMsg( DisconnectPkg::EType /*eType*/ ){}
//     //-----------------------------------------------------------------------------------------------------------
//     virtual bool    Connect(const char* lpszAddress, int nPort, const char* pBuf = 0, const DWORD dwBufLen = 0 );
//     ///���ӷ������ɹ�ʱ����
//     virtual void    OnConnect() { InterlockedExchange( (LONG*)&m_bIsConnecting, (LONG)false ); m_bIsClosed = false; Recv(); }
//     virtual bool    IsConnected(){ return m_hSocket != INVALID_SOCKET && m_bIsConnecting == false; };
//     //-----------------------------------------------------------------------------------------------------------
//     virtual void    Send( const char* /*buf*/, int /*nLen*/ );
//     virtual void    Send( const std::string& strPkg )
//     { 
//         if( m_bIsClosed ) return;
//         if( !strPkg.empty() )
//         {
//             Send( strPkg.c_str(), (int)strPkg.size() ); 
//         }
//     }
//     virtual void    Send( const std::string* pStrPkg ){ Send( pStrPkg->c_str(), (int)pStrPkg->size() ); }
//     void            Send();
//     virtual void    OnSendSuccess(DWORD dwIOSize);
//     //-----------------------------------------------------------------------------------------------------------
//     virtual void    Recv();	//�޸�RecvΪ�麯����������̳�����Ը��ݾ���Ӧ�����̣��Ż������������Ĵ�С waj at 2012.8.15
// 	virtual void	OnRecvSuccess(DWORD dwIOSize);
//     ///�����ݵ���ʱ����
//     virtual int     OnRead(const char* /*pBuf*/, int /*nLen*/);
//     virtual void    PostMsg( const char* /*pBuf*/, int /*nLen*/ ){};
//     //-----------------------------------------------------------------------------------------------------------
//     int             GetSocketOption(int level, int optname, char* optval, int* optlen)
//     {
//         return getsockopt(m_hSocket, level, optname, optval, optlen);
//     }
//     int             SetSocketOption(int level, int optname, const char* optval, int optlen)
//     {
//         return setsockopt(m_hSocket, level, optname, optval, optlen);
//     }      
//     ///IO Socket Option������
//     virtual void    OnIOCtrl() {}
//     //-----------------------------------------------------------------------------------------------------------
//     ///������
//     virtual void    OnError(int /*nErrCode*/,const char * /*pszErrInfo*/);// {}    
// 
//     SOCKET          GetSocket() { return m_hSocket; }
//     void            SetSocket( SOCKET aSock );
//     //-----------------------------------------------------------------------------------------------------------
//     void			SetAddr( const char* pszIp, USHORT uPort ){ m_strIp = pszIp; m_nIp = inet_addr( m_strIp.c_str() ); m_nPort = uPort; }
//     const char*		GetAddrIp()const { return m_strIp.c_str(); }
//     long            GetAddrIpInNum()const { return m_nIp; }
//     USHORT			GetAddrPort() const { return (USHORT)m_nPort; }
//     //-----------------------------------------------------------------------------------------------------------
//     void            SetIsSending( bool bIsSending ){ InterlockedExchange( (LONG*)&m_bIsSending, (LONG)bIsSending ); }
//     void            SetIsClosePending( bool bIsClosePending ){ InterlockedExchange( (LONG*)&m_bClosePending, (LONG)bIsClosePending ); }
// 
// 	int				GetSendBufferLen(){return m_sendBuf.GetQueueLen();
// 	}
// protected:
//     SOCKET              m_hSocket;
//     //SOCKET              m_hSocketConnect;
// 	YTSvrLib::CLock               m_sendLock;
//     OverlappedPlus      m_olSend;
//     CSendBuffer			m_sendBuf;	//CBuffer< 1024*16 >  m_sendBuf;
// 
//     // recv struct  
//     OverlappedPlus      m_olRecv;
//     CBuffer< 1024*8 >   m_recvBuf;    
// 
//     int                 m_nPort;
//     sockaddr_in         m_addr;
//     sockaddr_in         m_addrConnect;
//     std::string         m_strIp;
//     long                m_nIp;
// 
//     DISCONNECTOVERLAP   m_olDisConnect;
//     CONNECTOVERPLUS     m_olConnect;
//     ACCEPTOVERPLUS      m_olAccept;
// 
//     LPFN_ACCEPTEX	            m_lpfnAcceptEx;
//     LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
// 
//     LPFN_CONNECTEX	    m_lpfnConnectEx;
//     LPFN_DISCONNECTEX   m_lpfnDisconnectEx;
//     //bool                m_bFree;
// 
//     bool                m_bClosePending;
//     bool                m_bIsSending;
//     bool                m_bIsConnecting;
//     bool                m_bIsClosed;
//     bool                m_bIsDisconnecting;
// 
// public:
//     ISocketBase()
//         :
//     m_hSocket(INVALID_SOCKET),
//         //m_hSocketConnect(INVALID_SOCKET),
//         //m_bFree(true),
//         m_nPort(0)        
//     {
//         Init();
//     }
// 
//     virtual void Init()
//     {
//         m_lpfnAcceptEx = 0;
//         m_lpfnGetAcceptExSockaddrs = 0;
//         m_hSocket = (INVALID_SOCKET);
//         //m_bFree = (true);
//         m_bClosePending = false;
//         m_bIsSending = false;
//         m_bIsConnecting = false;
//         m_bIsDisconnecting = false;
//         m_bIsClosed = false;
//         m_strIp.clear();
//         m_nPort = (0);
//         memset( &m_olSend, 0, sizeof(m_olSend) );
//         memset( &m_olRecv, 0, sizeof(m_olRecv) );
//         memset( &m_olConnect, 0, sizeof(m_olConnect) );
//         memset( &m_olAccept, 0, sizeof(m_olConnect) );
//         memset( &m_addr, 0, sizeof(m_addr) );
//         memset( &m_addrConnect, 0, sizeof(m_addrConnect) );    
//         m_olRecv.SetType( IORecv );
//         m_olSend.SetType( IOSend );
//         m_sendBuf.Clear();
//         m_recvBuf.Clear();   
//         m_lpfnConnectEx = 0;
//         m_lpfnDisconnectEx = 0;
//     }
// };
// 
// //class CSocketPool :public CPool<ISocketBase>
// //{
// //    static CSocketPool* m_pInstance;
// //    CSocketPool(){};
// //    ~CSocketPool(){}
// //
// //    //CPool<ISocketBase> m_pool;
// //public:
// //    static CSocketPool* GetInstance()
// //    {
// //        if( !m_pInstance )
// //        {
// //            m_pInstance = new CSocketPool;
// //        }
// //
// //        return m_pInstance;
// //    }
// //
// //    ISocketBase* ApplyNewAcceptedSocket( const char* pszIp, USHORT uPort )
// //    {
// //        if( ISocketBase* pSocket = ApplyObj() )
// //        {
// //            pSocket->InitSocket();
// //            pSocket->SetAddr( pszIp, uPort );
// //
// //            return pSocket;
// //        }
// //        return 0;
// //    }
// //};
// #endif
// 
// #ifdef LIB_LINUX

class CSocketMgr;
class ISocketBase
{
public:
	ISocketBase()
	{
		m_pEventBuffer = NULL;
		m_pSocketParser = NULL;
		m_nSocket = 0;
		m_nPort = 0;
		ZeroMemory(m_szDstIP, sizeof(m_szDstIP));
		m_recvBuf.Clear();

		m_bIsClosed = false;
		m_bIsSending = false;
		m_bIsConnecting = false;
		m_bIsDisconnecting = false;
	}

	virtual ~ISocketBase()
	{
		
	}

	virtual void Init()
	{
		m_pEventBuffer = NULL;
		m_pSocketParser = NULL;
		m_nSocket = 0;
		m_nPort = 0;
		ZeroMemory(m_szDstIP, sizeof(m_szDstIP));
		m_recvBuf.Clear();

		m_bIsClosed = false;
		m_bIsSending = false;
		m_bIsConnecting = false;
		m_bIsDisconnecting = false;
	}

	void SetAddr(const char* pszIP, int nPort);

	virtual bool OnCreate(evutil_socket_t nSocket,CSocketMgr* pSocketParser,const char* szIP,int nPort);

	void OnRecvSuccess();
	virtual int OnRead(const char* pBuf, int nLen);
	virtual int Recv();

	virtual void Send(const char* buf,int len);
	void OnSend();

	virtual void OnError(int, const char*);

	virtual void OnClose();
	virtual bool Disconnect();
	virtual void OnDisconnect();
	virtual void SafeClose();

	bool Connect(const char* lpszAddress, int nPort);
	void OnConnected();
	virtual void OnConnect(){};

	virtual void ReclaimObj() = 0;
public:
	bufferevent* GetEventBuffer() {return m_pEventBuffer;}
	evutil_socket_t GetSocket(){return m_nSocket;}
	const char* GetCurIP(){return m_szDstIP;}
	const char* GetAddrIp()
	{
		return m_szDstIP;
	}
	int GetCurPort(){return m_nPort;}
	int GetAddrPort()
	{
		return m_nPort;
	}

	CSocketMgr* GetSocketParser()
	{
		return m_pSocketParser;
	}
protected:
	CSocketMgr* m_pSocketParser;
	bufferevent* m_pEventBuffer;
	evutil_socket_t m_nSocket;
	char m_szDstIP[32];
	int m_nPort;
	CBuffer<BLOCK_RECV_BUFFER_SIZE>   m_recvBuf;
	CSendBuffer m_sendBuf;
	YTSvrLib::CLock m_sendLock;
protected:
	bool m_bClosePending;
	bool m_bIsSending;
	bool m_bIsConnecting;
	bool m_bIsClosed;
	bool m_bIsDisconnecting;
};

class CSocketMgr
{
public:
	CSocketMgr()
	{
		m_pMainListener = NULL;
	}

	virtual ~CSocketMgr()
	{
		
	}

public:
	virtual bool StartListen(int nPort, const char* pstrIp);
	virtual bool StopListen();
	virtual void OnDisconnectEvent(){};
	evconnlistener* GetListener() {return m_pMainListener;}
public:
	virtual void ProcessAcceptedMsg(ISocketBase*){}
	virtual void ProcessDisconnectMsg(ISocketBase*){}
public:
	virtual ISocketBase* AllocateSocketHandleObject() = 0;
	virtual void ReclaimObj(ISocketBase* pSocket) = 0;
public:
	static void OnAccept(struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sock, int socklen, void *arg);
	static void OnRead(bufferevent *bev, void *arg);
	static void OnError(bufferevent *bev, short events, void *arg);
private:
	evconnlistener* m_pMainListener;
};

//#endif