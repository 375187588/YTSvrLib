/*MIT License

Copyright (c) 2016 Zhe Xu

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

#define YTSVRLIB_VERSION_CODE	1.0

#define APPLICATION_EVENT_MAX_COUNT 16	//���߳���ദ���¼���������ֵ���ܳ���64
#define APPLICATION_EVENT_MIN_COUNT 1	//���߳����������¼�������Ԥ������ͨ���¼�

#define USER_EVENT_MAX_COUNT	(APPLICATION_EVENT_MAX_COUNT-APPLICATION_EVENT_MIN_COUNT)	//�û����ɶ�����¼�����
#define EVENT_OVERTIME_CHECK_SPACE	50	//�¼�δ����ʱ�䳬����ֵʱ,�����¼��,��λ����

extern "C" {
	double get_version_code();
}

namespace YTSvrLib
{

	typedef void(*EventProc)();//�¼����������Ͷ���

	typedef struct _EVENTINFO
	{
		EventProc	Proc;		//�¼�������
		DWORD		dwLastHandleTime;//���һ�δ���ʱ��

		_EVENTINFO() :/*dwPeriod(0),*/ dwLastHandleTime(0)
		{}
	}EVENTINFO, *PEVENTINFO;

	class CServerApplication
	{
	public:
		explicit CServerApplication();
		~CServerApplication();
		//ϵͳ��ʼ������main()���ã�nEventCountΪ�¼�����(���벻����USER_EVENT_MAX_COUNT)�� ����false��ϵͳ����ʧ��
		//pAppExitEventName: �رճ���ʱ,���ø����Ƶ��¼�,�ɼ�س�������
		void GlobalInit();
		bool Init(int nEventCount, const char* pAppExitEventName);
		void Run();//���߳̽�������״̬�����ڵȴ��������¼�����main()����

		//================================ע���¼���������================================
		bool RegisterEvent(DWORD dwEventIndex, EventProc Proc);//�����¼�������
		void SetEvent(DWORD dwEventIndex);//�����¼�����������������Ч���������ٸú����ĵ��ô�����������������Ϣ�ﵽ������ֻ����Ϣ����Ϊ������µ��ã�
	private:
		bool InitEvent();//��ʼ���¼������ù����¼�
		bool CheckEventIndex(DWORD &dwEventIndex);//��dwEventIndexת������У��dwEventIndex�ĺϷ��ԣ������ظ����õľ��ִ�йرղ���
		bool SetEventInfo(DWORD dwEventIndex, HANDLE hEventHandle, EventProc Proc);
	private:
		int						m_nEventCount;	//�¼�����
		CHAR					m_szModuleName[MAX_PATH];//��������
		EVENTINFO					m_ayEventHandle[APPLICATION_EVENT_MAX_COUNT];// �¼�������

#ifdef LIB_WINDOWS
		HMODULE					m_hException;
		HMODULE					m_hZlib;
#endif // LIB_WINDOWS

		std::list<DWORD>		m_listEventQueue;// �¼�����
		YTSvrLib::CLock			m_lockQueue;// �¼�������
		YTSvrLib::CSemaphore	m_semQueue;// �¼������ź���
	};
}

