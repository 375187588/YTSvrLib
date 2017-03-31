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
#ifndef _XML_PARSER_H
#define _XML_PARSER_H

#include<string>

#include "tinyxml2.h"

using std::string;

class CXMLParser
{
public:

	CXMLParser(void)
	{
		m_xml.Clear();
		m_pElement = NULL;
	}

	~CXMLParser(void)
	{
		m_xml.Clear();
	}

private:

	tinyxml2::XMLDocument m_xml;

	tinyxml2::XMLElement* m_pElement;

private:

	tinyxml2::XMLElement* GetFirstElement(LPCSTR ElementMark, tinyxml2::XMLElement* pElement);

public:

	//����xml�ַ���
	bool ParseXmlStr(LPCSTR lpszXmlStr);

	//����xml�ļ�
	bool ParseXmlFile(LPCSTR lpszXmlFilePath);

	//���ݱ�ǩȡֵ
	tinyxml2::XMLElement* GetFirstElementValue(LPCSTR lpszElementMark, string& strValue);

	//���ͬһ��ǩ�ļ�¼ȡֵ
	tinyxml2::XMLElement* GetNextElementValue(tinyxml2::XMLElement* pElement, LPCSTR lpszElementMark, string& strValue);

	//ȡ������ֵ
	bool GetElementAttributeValue(tinyxml2::XMLElement* pElement,LPCSTR lpszAttributeName,string& strValue);

	//��ȡ�����
	tinyxml2::XMLElement* GetRootElement();

	//���ص�ǰ��xml�ַ���
	string GetXmlStr();

	//��ս���������(ע��:��Ҫ�Լ�delete.tinyxml�����delete���е�����)
	void Clear();

	//���һ�����ڵ�
	tinyxml2::XMLElement* AddXmlRootElement(LPCSTR lpszElementMark);

	//����ӽڵ�
	tinyxml2::XMLElement* AddXmlChildElement(tinyxml2::XMLElement* pElement, LPCSTR lpszElementMark);

	tinyxml2::XMLElement* AddXmlChildElement(tinyxml2::XMLElement* pElement,LPCSTR lpszElementMark,LPCSTR lpszValue);
	tinyxml2::XMLElement* AddXmlChildElement(tinyxml2::XMLElement* pElement,LPCSTR lpszElementMark,LPCWSTR lpwzValue);
	tinyxml2::XMLElement* AddXmlChildElement(tinyxml2::XMLElement* pElement,LPCSTR lpszElementMark,int nValue);
	tinyxml2::XMLElement* AddXmlChildElement(tinyxml2::XMLElement* pElement, LPCSTR lpszElementMark, LONGLONG nValue);
	tinyxml2::XMLElement* AddXmlChildElement(tinyxml2::XMLElement* pElement, LPCSTR lpszElementMark, long nValue);
	tinyxml2::XMLElement* AddXmlChildElement(tinyxml2::XMLElement* pElement, LPCSTR lpszElementMark, double dValue);

	//���ڵ����ֵ
	tinyxml2::XMLElement* AddElementValue(tinyxml2::XMLElement* pElement, LPCSTR lpszToAdd);

	//������Լ�����ֵ
	tinyxml2::XMLElement* AddXmlAttribute(tinyxml2::XMLElement* pElement,LPCSTR lpszAttributeKey,LPCSTR lpszAttributeValue);

	//�������
	tinyxml2::XMLDeclaration* AddXmlDeclaration(LPCSTR lpszVer,LPCSTR lpszEncode,LPCSTR lpszStandalone);

	//���ע��
	tinyxml2::XMLComment* AddXmlComment(tinyxml2::XMLElement* pElement,LPCSTR lpszComment);

	//��xml���ݱ��浽�ļ�
	bool SaveFile(LPCSTR lpszFileName);

};

#endif

