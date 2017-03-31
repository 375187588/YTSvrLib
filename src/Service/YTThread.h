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
#pragma once

namespace YTSvrLib
{

	const size_t THREAD_INFO_LEN = 128;

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
			return (char *) &m_ThreadInfo;
		}

	protected:
		///��¼�߳����͵Ļ��壬���߳��쳣�˳������������
		char    m_ThreadInfo[THREAD_INFO_LEN];
		virtual void SetThreadInfo()
		{};
	protected:

		virtual bool PreLoopFunc()
		{
			return true;
		}
		///�̵߳���������
		virtual void Execute(void) = 0;

		///Terminate�¼�
		virtual void OnTerminated(void)
		{};

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
		static void* ThreadProc(LPVOID pParam);
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
		CThreadPool(void)
		{}
		virtual ~CThreadPool(void)
		{
			if (m_vecThreads.size() > 0)
				ShutDown();
		}

		///��������߳�֮����������
		void Start(DWORD dwSleep = 0);

		///��ֹ������ֹͣ�����߳�
		void ShutDown();

		///��ȡ��ǰ���̸߳���
		size_t GetThreadCount()
		{
			return m_vecThreads.size();
		}

		///����һ���̶߳���
		void AddThread(CThread* pThread)
		{
			if (pThread) m_vecThreads.push_back(pThread);
		}

		///��ȡһ���̶߳���
		CThread* GetThread(size_t nIndex);
	private:
		///�Ѵ������̹߳������б�
		std::vector<CThread*> m_vecThreads;
	};
}