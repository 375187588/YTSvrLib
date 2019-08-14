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
#include "../stl/wqueue.h"

#define SNDBUFFER_BLOCK_SIZE	1024

namespace YTSvrLib
{
	struct YTSVRLIB_EXPORT sSndBufferBlock : public CRecycle
	{
		char m_szBlock[SNDBUFFER_BLOCK_SIZE];
		int m_nDataLen;

		virtual void Init();

		char* GetBlock();

		int GetDataLen();

		int SetData(LPCSTR pszData, int nLen);

		void OnSend(int nLength);
	};

	class YTSVRLIB_EXPORT CSendBuffer
	{
	public:
		CSendBuffer(void);
		virtual ~CSendBuffer(void);

		sSndBufferBlock* AllocateBlock();

		void ReleaseBlock(sSndBufferBlock* pObj);

		BOOL IsSending();

		///�򻺳���������һ�����ݣ�
		BOOL AddBuffer(const char* buf, int nSize);

		char* GetDataToSend();

		int GetDataLenToSend();

		BOOL OnSend(int nLength);

		void Clear();

		int GetQueueLen();

		void SetQueueLenMax(int nMax);

		BOOL IsQueueFulled();

	private:
		sSndBufferBlock* m_pBlockSending;
		CWQueue<sSndBufferBlock*> m_queueSnd;
		int m_nQueueLenMax;
		CPool<sSndBufferBlock, 64> m_poolBlock;
	};

	//////////////////////////////////////////////////////////////////////////

	template<size_t size>
	class CBuffer
	{
	public:
		//CBuffer(const CBuffer& rSrc){  }
		//void operator=(const CBuffer& rSrc);
		CBuffer() : m_nLength(0)
		{
			m_nBufSize = size;
			m_pbuf = new char[size];
			ZeroMemory(m_pbuf, size);
			m_nBufSizeMax = size * 16;
		}
		~CBuffer()
		{
			if (m_pbuf)
			{
				delete[] m_pbuf;
				m_pbuf = NULL;
			}
			m_nBufSize = 0;
		}
		CBuffer<size>(CBuffer<size>& other) : m_nLength(other.GetLength())
		{
			m_nBufSize = other.GetCapacity();
			m_pbuf = new char[m_nBufSize];
			memcpy(m_pbuf, other.GetBuffer(), other.GetLength());
			m_nBufSizeMax = other.GetBufSizeMax();
			m_nLength = other.GetLength();
		}

		///��ȡ��������������
		size_t GetCapacity()
		{
			return m_nBufSize;
		}
		void SetBufSizeMax(size_t nMax)
		{
			m_nBufSizeMax = nMax;
		}
		size_t GetBufSizeMax()
		{
			return m_nBufSizeMax;
		}
		BOOL ReSize(size_t nNewSize)
		{
			if (nNewSize < m_nLength)
				return FALSE;
			char* pNewBuf = new char[nNewSize];
			if (pNewBuf == NULL)
				return FALSE;
			ZeroMemory(pNewBuf, nNewSize);
			if (m_pbuf)
			{
				if (m_nLength > 0)
					memcpy(pNewBuf, m_pbuf, m_nLength);
				delete[] m_pbuf;
			}
			else
				m_nLength = 0;
			m_pbuf = pNewBuf;
			m_nBufSize = nNewSize;
			if (m_nBufSize > m_nBufSizeMax)
				m_nBufSizeMax = m_nBufSize;
			return TRUE;
		}

		///��ȡ��ǰ�����������������ݵĳ���
		size_t GetLength()
		{
			return m_nLength;
		}

		char * GetIdleBuffer()
		{
			return m_pbuf + m_nLength;
		}

		size_t GetIdleLength()
		{
			return m_nBufSize - m_nLength;
		}

		///�򻺳���������һ�����ݣ������Ӻ󻺳�����Խ�磬�������Ӳ�����ʧ��
		BOOL AddBuffer(const char* buf, size_t nSize)
		{
			if (m_nLength + nSize > m_nBufSize)
			{
				size_t nNeedSizeMin = m_nLength + nSize;
				size_t nNewBufSize = m_nBufSize + ((nNeedSizeMin - m_nBufSize) / size + 1)*size;
				if (nNewBufSize > m_nBufSizeMax)
					return FALSE;
				if (FALSE == ReSize(nNewBufSize))
					return FALSE;
			}
			memcpy((unsigned char*) &m_pbuf[m_nLength], buf, nSize);
			m_nLength += nSize;
			return TRUE;
		}

		//���ӻ����������ݵĳ���
		BOOL AddBuffer(size_t nSize)
		{
			if (m_nLength + nSize > m_nBufSize)
			{
				return FALSE;
			}
			m_nLength += nSize;
			return TRUE;
		}

		///��ȡ������ͷ�ĵ�ַ
		char* GetBuffer()
		{
			return m_pbuf;
		}

		///�ͷŴӻ�������ʼ��ָ�����ȵ����ݿռ�
		void ReleaseBuffer(size_t nLength)
		{
			if (nLength >= m_nLength)
			{
				m_nLength = 0;
			}
			else
			{
				if (nLength > 0)
				{
					m_nLength -= nLength;
					memcpy(&m_pbuf[0], &m_pbuf[nLength], m_nLength);
				}
				else
				{
					// ����û�û�д�����κ����ݣ�ͬʱ�������Ѿ����ˣ���ջ���
					if (m_nLength >= m_nBufSize)
					{
						m_nLength = 0;
					}
				}
			}
		}

		///�ָ���ʼ״̬
		void Clear()
		{
			m_nLength = 0;
		}
	private:
		size_t m_nLength;
		size_t m_nBufSize;
		size_t m_nBufSizeMax;
		char* m_pbuf;
	};
}