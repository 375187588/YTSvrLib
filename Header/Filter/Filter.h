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

/////////////////////////////////////////////////////////////////////////////
// CKeywordFilter : A keyword filter.�ؼ��ʹ���.
//

#pragma once

namespace YTSvrLib
{
	// wchar_t�ַ�������
	class CKeywordFilterW : public CSingle<CKeywordFilterW>
	{
	public:
		CKeywordFilterW()
		{

		}
		virtual ~CKeywordFilterW()
		{
			m_vctKeyword.clear();
			m_mpKeyword.clear();
		}
	public:
		void Init()
		{
			m_vctKeyword.clear();
			m_mpKeyword.clear();
		}

		// ���һ���ؼ���
		void AddKeyWord(wchar_t* pwzKeyWord);

		// Ԥ����.����������йؼ��ֺ�����Լ�������
		void Preprocess();

		// �ؼ�������.������pstrContent���ҵ������йؼ����м����Ϊ*��.����fuck������Ϊf**k
		void DoFilter(wchar_t* pstrContent, long nTextLen);
		
		// ������pstrContent���Ƿ��йؼ���.�ҵ��ĵ�һ���ؼ��ʽ�����pstrOutKey�����(��pstrOutKey��ΪNULL��nOutMaxLength�����㹻).
		BOOL HasKeyWord(const wchar_t* pstrContent, long nTextLen, wchar_t* pstrOutKey = NULL, long nOutMaxLength = 0);

	private:
		std::vector< std::wstring > m_vctKeyword; //Sorted.����ؼ���

		typedef std::pair< std::vector< std::wstring >::iterator, std::vector< std::wstring >::iterator > ITPAIR;

		std::map< wchar_t, ITPAIR> m_mpKeyword;// Keyword Index.�ؼ�������
	};

	class CKeywordFilterA : public CSingle < CKeywordFilterA >
	{
	public:
		CKeywordFilterA()
		{

		}
		virtual ~CKeywordFilterA()
		{
			m_vctKeyword.clear();
			m_mpKeyword.clear();
		}
	public:
		void Init()
		{
			m_vctKeyword.clear();
			m_mpKeyword.clear();
		}

		// ���һ���ؼ���
		void AddKeyWord(char* pszKeyWord);

		// Ԥ����.����������йؼ��ֺ�����Լ�������
		void Preprocess();

		// �ؼ�������.������pstrContent���ҵ������йؼ����м����Ϊ*��.����fuck������Ϊf**k
		void DoFilter(char* pstrContent, long nTextLen);

		// ������pstrContent���Ƿ��йؼ���.�ҵ��ĵ�һ���ؼ��ʽ�����pstrOutKey�����(��pstrOutKey��ΪNULL��nOutMaxLength�����㹻).
		BOOL HasKeyWord(const char* pstrContent, long nTextLen, char* pstrOutKey, long nOutMaxLength);

	private:
		std::vector< std::string > m_vctKeyword; //Sorted.����ؼ���

		typedef std::pair< std::vector< std::string >::iterator, std::vector< std::string >::iterator > ITPAIR;

		std::map< char, ITPAIR> m_mpKeyword;// Keyword Index.�ؼ�������
	};
}