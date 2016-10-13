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
#ifndef _XML_PARSER_H
#define _XML_PARSER_H

#include<string>

#include "tinyxml.h"

using std::string;

class CXMLParser
{
public:

	CXMLParser(void)
	{
		m_xml.Clear();
	}

	~CXMLParser(void)
	{
		m_xml.Clear();
	}

private:

	TiXmlDocument m_xml;

	TiXmlElement* m_pElement;

private:

	TiXmlElement* GetFirstElement(LPCSTR ElementMark,TiXmlElement* pElement);

public:

	//����xml�ַ���
	bool ParseXmlStr(LPCSTR lpszXmlStr);

	//����xml�ļ�
	bool ParseXmlFile(LPCSTR lpszXmlFilePath);

	//���ݱ�ǩȡֵ
	TiXmlElement* GetFirstElementValue(LPCSTR lpszElementMark,string& strValue);

	//���ͬһ��ǩ�ļ�¼ȡֵ
	TiXmlElement* GetNextElementValue(TiXmlElement* pElement,LPCSTR lpszElementMark,string& strValue);

	//ȡ������ֵ
	bool GetElementAttributeValue(TiXmlElement* pElement,LPCSTR lpszAttributeName,string& strValue);

	//��ȡ�����
	TiXmlElement* GetRootElement();

	//���ص�ǰ��xml�ַ���
	string GetXmlStr();

	//��ս���������(ע��:��Ҫ�Լ�delete.tinyxml�����delete���е�����)
	void Clear();

	//���һ�����ڵ�
	TiXmlElement* AddXmlRootElement(LPCSTR lpszElementMark);

	//����ӽڵ�
	TiXmlElement* AddXmlChildElement(TiXmlElement* pElement,LPCSTR lpszElementMark,LPCSTR lpszValue = NULL);
	TiXmlElement* AddXmlChildElementW(TiXmlElement* pElement,LPCSTR lpszElementMark,LPCWSTR lpwzValue);
	TiXmlElement* AddXmlChildElementInt(TiXmlElement* pElement,LPCSTR lpszElementMark,int nValue);

	//���ڵ����ֵ
	TiXmlText* AddElementValue(TiXmlElement* pElement,LPCSTR lpszToAdd);

	//������Լ�����ֵ
	TiXmlElement* AddXmlAttribute(TiXmlElement* pElement,LPCSTR lpszAttributeKey,LPCSTR lpszAttributeValue);

	//�������
	TiXmlDeclaration* AddXmlDeclaration(LPCSTR lpszVer,LPCSTR lpszEncode,LPCSTR lpszStandalone);

	//���ע��
	TiXmlComment* AddXmlComment(TiXmlElement* pElement,LPCSTR lpszComment);

	//��xml���ݱ��浽�ļ�
	bool SaveFile(LPCSTR lpszFileName);

};

#endif

