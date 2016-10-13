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

namespace YTSvrLib
{
#define PERFERMANCE_MICRO_SECONDS 1000000//΢�뼶��ʱ
#define PERFERMANCE_MILLI_SECONDS 1000//���뼶��ʱ
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���ܼ����(Ĭ��Ϊ΢��)
#ifdef LIB_WINDOWS
	class PerfermanceWatch
	{
	public:
		PerfermanceWatch(size_t nTimesize);

		~PerfermanceWatch(){}
	private:
		size_t m_nTimesize;
	private:
		LARGE_INTEGER m_begintick;
		LARGE_INTEGER m_endtick;
		LARGE_INTEGER m_costtick;
		LARGE_INTEGER m_lasttick;
	public:
		LONGLONG Start();

		LONGLONG Stop();

		LONGLONG GetCostTotal();

		LONGLONG CheckPoint(LPCSTR lpszInfo, ...);
	};
#else
	class PerfermanceWatch
	{
	public:
		PerfermanceWatch(int nTimesize);
		~PerfermanceWatch(){}
	private:
		size_t m_nTimesize;
	private:
		timeval m_begintick;
		timeval m_endtick;
		timeval m_lasttick;
	public:
		void Start();

		LONGLONG Stop();

		LONGLONG GetSpan(timeval& begin, timeval& end);

		LONGLONG GetCostTotal();

		void CheckPoint(LPCSTR lpszInfo, ...);
	};
#endif // LIB_WINDOWS

	// ��ʼһ���������ܼ��
#define BEGIN_WATCH(type) PerfermanceWatch pw(type);pw.Start()
	// ͨ������
#define CHECK_WATCH(s, ...) pw.CheckPoint(s, __VA_ARGS__)
	// �����������ܲ���
#define END_WATCH() pw.Stop()
}