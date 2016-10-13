#pragma once
#include "SocketBase.h"
#include <vector>

#ifdef LIB_WINDOWS

#include <Windows.h>
#include <mswsock.h>

class CAcceptExManager;
class CIOCPObj
{
	typedef struct tagConnectInfo
	{
		static const INT_PTR IP_MAX_BUFFER_SIZE = 60;
		int           nPort;                    //�˿�
		char          szIP[IP_MAX_BUFFER_SIZE]; //����IP�Ļ���������
		//����ǰ�ȴ�ϵͳ��Դ�ͷŵ�ʱ�� ��Ҫ�Ƿ�ֹ�����������ӣ����ϴ�
		//���ӵ�SOCKET��Դû���ͷŵ���ϵͳ��Դ���Ĺ⣬ϵͳ��������� 
		DWORD         dwSleepTime;              
		BOOL          bConnectOp;   ///��������Ƿ����

		tagConnectInfo():nPort(0),dwSleepTime(0),bConnectOp(true){ memset( szIP, 0, sizeof(szIP) ); }
	}SConnectInfo,*PSConnectInfo;

	static const INT_PTR THREADSPERCPU = 2;
	static const INT_PTR DW_WAIT_THREAD_QUIT = 6000;

protected:
	static CIOCPObj* m_pInstance;    
	CIOCPObj()
	{
		//m_hSocket = INVALID_SOCKET;
		m_bFree = FALSE;
		m_ConnectInfo = NULL;
	}
	virtual ~CIOCPObj()
	{
		if(m_ConnectInfo != NULL)
		{
			delete m_ConnectInfo;
		}
	}

public:

	static CIOCPObj* GetInstance()
	{
		if( m_pInstance )
		{
			return m_pInstance;
		}
		m_pInstance = new CIOCPObj;
		return m_pInstance;
	}
	static void DelInstance()
	{
		if( m_pInstance )
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	bool StartUp();
	void ShutDown();


	//CAcceptExManager* GetAcceptManager(){ return m_pAcceptManager; }
	//void SetAcceptManager( CAcceptExManager* pAcceptMgr ){ m_pAcceptManager = pAcceptMgr; }


	///��ʼ����һ���˿�
	BOOL StartListen(int nPort);

	///ֹͣ�����˿�
	BOOL StopListen();

	///��ʼ����
	BOOL Connect(const char* lpszAddress, int nPort);

	///�Ͽ�����
	BOOL Disconnect();

	///global init and shutdown
	static BOOL Init(HINSTANCE hInstance);


	BOOL IsFree() const
	{
		return m_bFree;
	}

protected:
	void CloseSocket();

	//virtual void InitSocket()=0; 
	//virtual void Recv()=0; 

	//virtual void OnReadEvent(DWORD dwIOSize)=0; 
	//virtual void OnSendEvent(DWORD dwIOSize)=0; 

	//void AsynConnect();             ///���첽�ص������е��õĺ���
	PSConnectInfo   m_ConnectInfo;   ///������Ҫ���ӵ���Ϣ
	__inline BOOL DoFree()
	{
		return ::InterlockedExchange((LONG *)&m_bFree,TRUE)==FALSE;
	}

	__inline void SetNoFree()
	{
		::InterlockedExchange((LONG *)&m_bFree,FALSE);
	}

protected:
	volatile   BOOL         m_bFree;

public:

	static unsigned WINAPI ThreadPoolProc(LPVOID pParam);
	static bool AssociateSocketWithIoCP(SOCKET sock, ISocketBase* pIoCPBase);
	//static DWORD WINAPI ConnectToServer(void * pValue);


	static HANDLE               m_hCompletionPort;
	static UINT/*INT_PTR*/              m_nNumberOfWorkers;
	static std::vector<HANDLE>  m_vctWorkerThread;
	static HANDLE               m_hListenThread;

private:

	//CAcceptExManager  *     m_pAcceptManager;    
};
#endif
//class CAcceptExManager
//{
//private:
//    static CAcceptExManager* m_pInstance;
//    //CAcceptExManager(){};
//    //~CAcceptExManager(){};
//public:
//    static CAcceptExManager* GetInstance()
//    {
//        if( !m_pInstance )
//        {
//            m_pInstance = new CAcceptExManager;
//        }
//        return m_pInstance;
//    }
//protected:
//    static const INT_PTR POST_ACCEPTEX_COUNT = 128;
//public:
//    ///PostCount��ʼ��ʱ��Ͷ�ݵ�acceptex�ĸ���
//    explicit CAcceptExManager(int PostCount=POST_ACCEPTEX_COUNT)
//    {
//        m_PostCount = PostCount;
//        m_bFree = FALSE;
//    }
//
//    void Init( ISocketBase* pIoCPSocket )
//    {
//        m_pIoCPSocket = pIoCPSocket;
//        m_ListenSocket = pIoCPSocket->GetSocket();
//        __init();
//    }
//
//    virtual ~CAcceptExManager()
//    {
//        __Cleanup();
//    }
//
//    void Cleanup()
//    {
//        ::InterlockedExchange((LONG *)&m_bFree,TRUE);
//    }
//
//    void HandleAcceptExEvent( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//    void HandleAcceptExError( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//
//private:
//
//    int                         m_AddrLen;
//    SOCKET                      m_ListenSocket;
//
//    volatile    BOOL            m_bFree;
//    volatile    int             m_PostCount;
//
//    ISocketBase*                m_pIoCPSocket;
//
//    LPFN_ACCEPTEX	            m_lpfnAcceptEx;
//    LPFN_GETACCEPTEXSOCKADDRS   m_lpfnGetAcceptExSockaddrs;
//
//    ISocketBase::PACCEPTOVERPLUS   m_pAcceptExArray;
//public:
//    void __init();
//    void __Cleanup();
//    void PostAcceptEx( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//};
//
//class CConnectExManager
//{
//private:
//    static CConnectExManager* m_pInstance;
//    static const INT_PTR POST_CONNECTEX_COUNT = 128;
//public:
//    static CConnectExManager* GetInstance()
//    {
//        if( !m_pInstance )
//        {
//            m_pInstance = new CConnectExManager;
//        }
//        return m_pInstance;
//    }
//
//    explicit CConnectExManager(int PostCount=POST_CONNECTEX_COUNT)
//    {
//        m_PostCount = PostCount;
//        m_bFree = FALSE;
//    }
//
//    void Init( ISocketBase* pIoCPSocket, const char* szServerIp, USHORT uServerPort )
//    {
//        m_pIoCPSocket = pIoCPSocket;
//        m_hConnectSocket = pIoCPSocket->GetConnectSocket();
//        m_strServerIp = szServerIp;
//        m_nServerPort = uServerPort;
//        __init();
//    }
//
//    virtual ~CConnectExManager()
//    {
//        __Cleanup();
//    }
//
//    void Cleanup()
//    {
//        ::InterlockedExchange((LONG *)&m_bFree,TRUE);
//    }
//
//    void HandleConnectExEvent( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//    void HandleConnectExError( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//
//private:
//
//    int                         m_AddrLen;
//    SOCKET                      m_hConnectSocket;
//
//    volatile    BOOL            m_bFree;
//    volatile    int             m_PostCount;
//
//    ISocketBase*                m_pIoCPSocket;
//
//    LPFN_CONNECTEX	            m_lpfnConnectEx;
//
//    ISocketBase::PCONNECTOVERPLUS   m_pConnectEx;
//
//    std::string                 m_strServerIp;
//    USHORT                      m_nServerPort;
//public:
//    void __init();
//    void __Cleanup();
//    void PostConnectEx( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//};
//
//class CDisconnectExManager
//{
//private:
//    static CDisconnectExManager* m_pInstance;
//public:
//    static CDisconnectExManager* GetInstance()
//    {
//        if( !m_pInstance )
//        {
//            m_pInstance = new CDisconnectExManager;
//        }
//        return m_pInstance;
//    }
//
//    explicit CDisconnectExManager():m_lpfnDisconnectEx(0){}
//
//    void Disconnect( ISocketBase* pIoCPSocket, SOCKET hSocket );
//
//    virtual ~CDisconnectExManager(){}
//
//    void HandleDisconnectExEvent( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//    void HandleDisconnectExError( ISocketBase::OVERLAPPEDPLUS * pOverlapped);
//
//private:
//
//    LPFN_DISCONNECTEX	            m_lpfnDisconnectEx;
//};