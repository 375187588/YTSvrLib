#pragma once



//using namespace std;

//���ļ��е����ж��嶼��namespace LZCore��

const size_t THREAD_INFO_LEN=128;
/**
@brief ��װWindows�߳�
*
*	��װWindows�߳�
*/
class CThread //: public CRecycle
{
public:
	CThread(void);
	virtual ~CThread(void);
	///�����߳�
	virtual BOOL Start();

	///��ֹ�߳�
	void Terminate(void);

	///�ж��߳��Ƿ�����ֹ
	BOOL IsTerminated(void);

	///�߳��Ƿ�������
	BOOL IsStarted(void);

	///�ȴ��߳��˳�
	void WaitFor(void);

	const char * GetThreadInfo() 
	{
		return (char *)&m_ThreadInfo;
	}

protected:
	///��¼�߳����͵Ļ��壬���߳��쳣�˳������������
	char    m_ThreadInfo[THREAD_INFO_LEN];
	virtual void SetThreadInfo(){};
protected:

    virtual bool PreLoopFunc(){return true;}
	///�̵߳���������
	virtual void Execute(void) = 0;

	///Terminate�¼�
	virtual void OnTerminated(void) {};

	///��־�Ƿ�����ֹ
	volatile BOOL m_bTerminated;

private:
	//�߳�ID
	DWORD m_dwThreadID;
#ifdef LIB_WINDOWS
	///��־�Ƿ��ѹ���
	BOOL m_bIsSuspended;
	///�߳̾��
	HANDLE m_hThread;
#endif // LIB_WINDOWS

#ifdef LIB_WINDOWS
	static unsigned WINAPI ThreadProc(LPVOID pParam);
#else
	static void ThreadProc(LPVOID pParam);
#endif // LIB_WINDOWS
};

/**
@brief �������̶߳���
*
*	�������̶߳���
*/

class CThreadPool
{
public:
	CThreadPool(void) {}
	virtual ~CThreadPool(void)
	{
		if (m_vecThreads.size() > 0)
			ShutDown();
	}

	///��������߳�֮����������
	void Start( DWORD dwSleep = 0 )
	{
		for (unsigned int i = 0; i < m_vecThreads.size(); i++)
		{
			CThread* pThread = m_vecThreads[i];
			if (!pThread->IsStarted())
			{
				pThread->Start();
				Sleep( dwSleep );
			}
		}
	}

	///��ֹ������ֹͣ�����߳�
	void ShutDown()
	{
		for (unsigned int i = 0; i < m_vecThreads.size(); i++)
			m_vecThreads[i]->Terminate();
		while(m_vecThreads.size() > 0)
		{
			CThread* pThread = m_vecThreads[0];
			pThread->WaitFor();
			delete pThread;
			m_vecThreads.erase(m_vecThreads.begin());
		}
	}

	///��ȡ��ǰ���̸߳���
	size_t GetThreadCount() { return m_vecThreads.size(); }

	///����һ���̶߳���
	void AddThread(CThread* pThread) { if (pThread) m_vecThreads.push_back(pThread); }

	///��ȡһ���̶߳���
	CThread* GetThread(size_t nIndex)
	{
		if (nIndex >= m_vecThreads.size())
			return NULL;
		return m_vecThreads[nIndex];
	}

private:
	///�Ѵ������̹߳������б�
	std::vector<CThread*> m_vecThreads;
};