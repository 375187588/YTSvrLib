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
#ifndef __UTILITY_H_
#define __UTILITY_H_

//---------------------------------------
//
//	bool IsDirExist(char *lpszDir)
//
//	����:	�ж��ļ�Ŀ¼lpszDir�Ƿ����
//
//	����:	lpszDir	-- Ŀ¼�ַ���
//			
//
//	����:	true -- ����
//			false-- ������
//		
//---------------------------------------
bool IsDirExist(char *lpszDir);

//================================�ļ�������=======================================
#ifdef LIB_WINDOWS
inline void StringUpr(std::string& s)
{
	_strupr_s((char*)(s.c_str()), s.length());
}
inline void StringLwr(std::string& s)
{
	_strlwr_s((char*)(s.c_str()), s.length());
}
#endif // LIB_WINDOWS

//-------------------------------------------------
//
//	char* Trim(char *lpszStr)
//
//	����:	��ȥ�ַ���lpszStrͷβ�Ŀ��ַ�
//			���ַ����� ' ','\f','\r','\n','\t','\v'
//
//	����:	lpszStr -- Ҫ������ַ���			
//
//	����:	�������ִ�
//
//-------------------------------------------------
char* Trim(char* lpszStr );
wchar_t* TrimW(wchar_t* lpswStr );

//--------------------------------------------------
//
//	char* TrimR(char *lpszStr)
//
//	����:	��ȥ�ַ���lpszStrβ���Ŀ��ַ�
//			���ַ����� ' ','\f','\r','\n','\t','\v'
//
//	����:	lpszStr -- Ҫ������ַ���
//
//	����:	�������ִ�
//
//--------------------------------------------------
char* TrimR(char* lpszStr);
wchar_t* TrimRW(wchar_t* lpswStr);


//--------------------------------------------------
//
//	char* TrimL(char *lpszStr)
//
//	����:	��ȥ�ַ���lpszStrͷ���Ŀ��ַ�
//			���ַ����� ' ','\f','\r','\n','\t','\v'	
//
//	����:	lpszStr -- Ҫ������ַ���	
//
//	����:	�������ִ�
//
//--------------------------------------------------
char* TrimL(char* lpszStr );
wchar_t* TrimLW(wchar_t* lpswStr );

//--------------------------------------------------
//	����:	��lpszDelimiter�а������ַ��ָ��ִ�lpszStr,
//			�õ��ķָ��Ӵ��������ַ�������vecResult��
//
//	����:	lpszStr			-- [in]Ҫ�ָ���ַ���
//			lpszDelimiter	-- [in]�ָ��ַ���
//			vecResult		-- [out]���ص��ַ�������
//
//	����:	��
//		
//-----------------------------------------------
void StrDelimiter(const char *lpszSrc, const char *lpszDelimiter, std::vector<std::string> &vecResult);
void StrDelimiter( std::string& strSrc, const char* lpzDelimiter, std::vector<std::string> &vctResult );
void StrDelimiter(const char *lpszSrc, const char *lpszDelimiter, std::vector<int> &vctResult);
void StrDelimiter(std::string& strSrc, const char *lpszDelimiter, std::vector<int> &vctResult);

void StrDelimiter( const wchar_t* lpwzSrc, const wchar_t* lpwzDelimiter, std::vector<std::wstring> &vctResult );
void StrDelimiter( std::wstring& wstrSrc, const wchar_t* lpwzDelimiter, std::vector<std::wstring> &vctResult );
void StrDelimiter(const wchar_t* lpwzSrc, const wchar_t* lpwzDelimiter, std::vector<int> &vctResult);
void StrDelimiter( std::wstring& wstrSrc, const wchar_t* lpwzDelimiter, std::vector<int> &vctResult);
//��ֲ�����


//	����:	����ַ�
//	����:	pBuf			-- [in out]Ҫ�����ַ���,����ȷ�������㹻
//			cDelimiter		-- [in]�ָ��־�ַ�
//			cFill			-- [int]�����ַ�
void FillSpacing(char* pBuf, char cDelimiter, char cFill);

//-----------------------------------------------
//
//	char *DelSpace(char *s)
//
//	����:
//		���ַ���s�е�ǰ���հ׷���β��հ׷�ɾ����
//		�����ַ����м����������հ׷�ɾ��Ϊһ���հ׷�
//	����:
//		s -- Ҫ������ַ���
//	����:
//		�������ַ���
//
//-----------------------------------------------
char *DelSpace(char *s);

//-----------------------------------------------
//
//	char *DelSpace(char *s)
//
//	����:
//		������������ת��Ϊ�ִ�����
//	����:
//		pData -- ������ʼ��ַ
//		len   -- ���ݳ���
//		delim -- ����ִ�
//	����:
//		�������ַ���
//
//-----------------------------------------------
std::string HexDump(const BYTE* pData, int len, const char *delim=" ");

//-----------------------------------------------
//	����:
//		�ִ��滻
//	����:
//		strSource		-- ��Ҫ�任��ԭ�ִ�
//		strOldstring	-- ��Ҫ���滻�����ִ�
//		strNewstring	-- �滻�����ִ�
//	����:
//		�������ַ���
//
//-----------------------------------------------
void StrReplace(char *szSource, int nLen,const char *pszOldstring,const char *pszNewstring);
void StrReplaceW(wchar_t *wzSource, int nLen,const wchar_t *pwzOldstring,const wchar_t *pwzNewstring);

void StrReplace(std::string&strSource,const std::string&strOldstring,const std::string&strNewstring);
void StrReplaceW(std::wstring&strSource,const std::wstring&strOldstring,const std::wstring&strNewstring);

#ifdef LIB_WINDOWS
BOOL WCharToMByte( LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);
BOOL MByteToWChar(LPCSTR lpszStr, LPWSTR lpcwszStr, DWORD dwSize);
#endif // LIB_WINDOWS

// ���ֽ�ǿתANSI��ʽ�ַ���.ע��.���벻��
BOOL WChar2Ansi( LPCWSTR lpcwszStr, LPSTR lpszStr, int nStrLenMax );
// ANSI�ַ���ǿת���ֽ��ַ���.ע��.���벻��
BOOL Ansi2WChar( LPCSTR lpszStr, LPWSTR lpwszStr, int nWStrLenMax );

template<class T>
T S2Var(const std::string& strValue)
{
    T Result;
    std::stringstream s;
    s = strValue;
    s >> Result;
    return Result;
}

template<class T>
std::string Var2S(const T nValue)
{
    std::string Result;
    std::stringstream s;
    s << nValue;
	Result = s;
    return Result;
}

//================================��ѧ���㺯��================================

//------------------------------------------------------
//
//	int Random(int nFirst,int nEnd)
//	
//	����:	ȡnFirst��nEnd֮��������(����nFirst,nEnd)
//		
//	����:	nFirst -- ��ʼֵ
//			nEnd   -- ����ֵ	
//
//	����:	int	-- �õ��������
//
//------------------------------------------------------
int Random(int nFirst,int nEnd);

//------------------------------------------------------
//
//	unsigned int Random(unsigned int nEnd)
//	
//	����:	ȡ0��nEnd-1֮��������
//		
//	����:	nEnd   -- ����ֵ
//
//	����:	unsigned int	-- �õ������������ֵ����0x1000
//
//------------------------------------------------------
unsigned int Random(unsigned int nEnd);

#ifdef LIB_WINDOWS
bool InitGenRandomFunction();
bool ReleaseGenRandomFunction();
#endif

int Random2( int nMax, int nMin = 0 ); //���� nMin~nMax-1�������
LONGLONG Random2(LONGLONG nMax, LONGLONG nMin = 0);//���� nMin~nMax-1�������
DOUBLE Random2(DOUBLE dMax, DOUBLE dMin = 0.000000, int nPrecision = 0);//���� dMin~dMax�������.����ȡnPrecision

//================================ʱ�亯��================================

//-----------------------------------------------------------
//
//	char* GetDateTime(char *lpszTimeBuf,char Flag='A')
//
//	����:
//		�õ���ǰ����ʱ�䱣�浽�ַ���lpszTimeBuf��
//	����:
//		lpszTimeBuf	-- [out]Ҫ��������ʱ����ַ���(����Ҫע��)	
//		Flag -- 'A' lpszTimeBuf = '2003-10-20 11:20:35_233212'
//				'S' lpszTimeBuf = '10-20 11:20:35_233212'
//				'D' lpszTimeBuf = '2003-10-20'
//				'T' lpszTimeBuf = '11:20:35_233212'
//				'W' lpszTimeBuf = '2003-10'
//				default lpszTimeBuf = '2003-10-20 11:20:35'
//	����:
//		�õ���ʱ�������ַ���
//
//-----------------------------------------------------------
char* GetDateTime(char *lpszTimeBuf, int nLen, char Flag='A');
wchar_t* GetDateTimeW(wchar_t *lpwzTimeBuf, int nLen, char Flag='A');

//�ж�һ���ǲ��ǵ��µĵ�һ��
bool IsFirstDayOfMonth( const time_t& tTime );

//�ж�һ���ǲ��ǵ��µ����һ��
bool IsLastDayOfMonth( const time_t& tTime );

//ȡ��nBegin��ʼ������ΪnSize��һ��������ظ���
std::vector<int> ProduceRandSerial(int nBegin, int nSize);

//�����쳣
#ifdef LIB_WINDOWS
//�쳣����ص�����
LONG __stdcall TheCrashHandlerFunction ( EXCEPTION_POINTERS * pExPtrs );

//================================�����쳣�궨��================================
//�Ƿ񲶻��쳣
#define CATCH_EXCEPTION

#ifndef CATCH_EXCEPTION
#define TRY_BEGIN_EXCEP
#define TRY_END_EXCEP(symbol)
#else

#define TRY_BEGIN_EXCEP			__try {

#define TRY_END_EXCEP(symbol)		 }__except ( TheCrashHandlerFunction ( GetExceptionInformation ( ) ) )\
{\
    LOG( symbol);\
}
#endif

void LogRaiseException();

#define SELF_ASSERT(a, b)	{ if (!(a))	\
{	\
    LOG(__FILE__, __LINE__, "ASSERT����");	\
    LogRaiseException();	\
    b;	\
}}
#else
#define TRY_BEGIN_EXCEP try{

#define TRY_END_EXCEP(symbol) }catch(...){LOG(symbol);}
#endif // LIB_WINDOWS

//��ʱ��־
#define LOG_DECLARE_BEGIN	DWORD dwLogBeginTime = GetTickCount();
#define LOG_BEGIN			dwLogBeginTime = GetTickCount();
#define LOG_END(t, m)		if (GetTickCount()-dwLogBeginTime >= t)	\
{	\
    LOG(__FILE__, __LINE__, "%s : %d", m, GetTickCount()-dwLogBeginTime);	\
}




void InitlogManager( LPCSTR pszFilePrefix );
void DelLogManager();

#ifdef LIB_WINDOWS
__time32_t SystemTimeToTime_t( LPSYSTEMTIME pst );
#endif

int GetCPUCoreCount();// ��ȡcpu������

int lwchartoutf8(LPCWSTR p, LPSTR pdst, int cbMultiByte);
DWORD WINAPI    GetPrimeNumInRange(DWORD dwLow, DWORD dwHigh);


#endif