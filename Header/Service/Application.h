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

/////////////////////////////////////////////////////////////////////////////
// CServerApplication : Core Instance.���������̵�Ψһ����Լ��¼���ѭ��
//

#pragma once

#define YTSVRLIB_VERSION_CODE	1.01

#define APPLICATION_EVENT_MAX_COUNT 16	//main thread event max count.���߳��¼��������
#define APPLICATION_EVENT_MIN_COUNT 1	//main thread event min count.���߳��¼���С����

#define USER_EVENT_MAX_COUNT	(APPLICATION_EVENT_MAX_COUNT-APPLICATION_EVENT_MIN_COUNT)	//max user event count.�û����Զ�����¼�����

namespace YTSvrLib
{
	YTSVRLIB_EXPORT void SetPrivateLastError(UINT nErrorCode);
	YTSVRLIB_EXPORT UINT GetPrivateLastError();

	typedef void(*EventProc)();//event handle function define

	typedef struct _EVENTINFO
	{
		EventProc	Proc;		//event handler
		DWORD		dwLastHandleTime;//last handle time.���һ�δ���ʱ��
		_EVENTINFO() :dwLastHandleTime(0){
			Proc = NULL;
		}
	}EVENTINFO, *PEVENTINFO;

	class YTSVRLIB_EXPORT CServerApplication : public CSingle<CServerApplication>
	{
	public:
		explicit CServerApplication();
		virtual ~CServerApplication();

		// call it to enter event loop.waiting for event to handle.make sure all config has been prepaired.���߳̽�������״̬�����ڵȴ��������¼�����main()����.����֮ǰȷ�����ö��Ѿ�׼�����.
		void Run();

		// regist a event to event loop �����¼�������
		bool RegisterEvent(DWORD dwEventIndex, EventProc Proc);

		// add a new event to event loop.and activate the event.
		void SetEvent(DWORD dwEventIndex);
	private:
		// ��ʼ��������
		void GlobalInit();

		// set the event count .init the event pool.
		bool Init();

		// init event pool.
		bool InitEvent();

		// check the dwEventIndex available.
		bool CheckEventIndex(DWORD &dwEventIndex);

		// set the event.
		bool SetEventInfo(DWORD dwEventIndex, EventProc Proc);
	private:
		CHAR					m_szModuleName[MAX_PATH];//process name.������
		EVENTINFO					m_ayEventHandle[APPLICATION_EVENT_MAX_COUNT];// event handle map. �¼���

#ifdef LIB_WINDOWS
		HMODULE					m_hException;
#endif // LIB_WINDOWS

		std::list<DWORD>		m_listEventQueue;// event queue.�¼�����
		YTSvrLib::CLock			m_lockQueue;// lock for event queue.�¼�������
		YTSvrLib::CSemaphore	m_semQueue;// sem for event queue.�¼������ź���
	};
}

