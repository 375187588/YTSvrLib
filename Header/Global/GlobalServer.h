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
#include <math.h>

//ʱ����ض���
#define SEC_MINUTE	(60) // һ����
#define SEC_HOUR	(60*SEC_MINUTE) //һСʱ
#define SEC_DAY		(24*SEC_HOUR) //һ��
#define SEC_MONTH	(31*SEC_DAY) // һ����
#define SEC_YEAR	(12*SEC_MONTH) //һ��

#define SYS_WEEK_SUNDAY		0 //����
#define SYS_WEEK_MONDAY		1 //��һ
#define SYS_WEEK_TUESDAY	2 //�ܶ�
#define SYS_WEEK_WEDNESDAY	3 //����
#define SYS_WEEK_THURSDAY	4 //����
#define SYS_WEEK_FRIDAY		5 //����
#define SYS_WEEK_SATURDAY	6 //����

// ������������4�ֽڵ���ֵ�ϳ�Ϊһ��8�ֽڵ���ֵ
#define MAKELONGLONG(a, b)  ((LONGLONG)(((UINT)(((LONGLONG)(a)) & 0xffffffff)) | ((LONGLONG)((UINT)(((LONGLONG)(b)) & 0xffffffff))) << 32))
// ȡ8�ֽ����еĵ�λ
#define LOUINT(l)           ((UINT)(((LONGLONG)(l)) & 0xffffffff))
// ȡ8�ֽ����еĸ�λ
#define HIUINT(l)           ((UINT)((((LONGLONG)(l)) >> 32) & 0xffffffff))

// ȡ��Сֵ
template<typename _Ty>
inline _Ty _min(_Ty nLeft, _Ty nRight)
{
	return (nLeft < nRight) ? nLeft : nRight;
}

// ȡ���ֵ
template<typename _Ty>
inline _Ty _max(_Ty nLeft, _Ty nRight)
{
	return (nLeft > nRight) ? nLeft : nRight;
}

// ������ֵ
template<typename _Ty>
inline void _swap(_Ty& nLeft, _Ty& nRight)
{
	_Ty tmp;
	tmp = nLeft;
	nLeft = nRight;
	nRight = tmp;
}

typedef enum
{
	eInvalid, 
	eDisconnect, 
	eConnect, 
	eConnectFail, 
	eAccepted, 
	eCount
} EType;

typedef	std::vector<std::string> CStringVct;

enum EM_LOG_LEVEL
{
	LOG_LEVEL_TRACE,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
};

void LogBin( const char* pszData, int nDataLen );
void LogErrorASync(const char* szFile, const int nLine, EM_LOG_LEVEL emLevel, const char *fmt, ...);
void  LogError( const char* szFile, const int nLine, const char *fmt, ...);
void  LogError( const char *fmt, ...);
void  LogCommon( const char *fmt, ... );


#ifdef LIB_WINDOWS
#define LOG(s, ...) LogErrorASync( __FILE__, __LINE__,LOG_LEVEL_DEBUG, s, __VA_ARGS__)
#define LOGWARN(s,...) LogErrorASync( __FILE__, __LINE__,LOG_LEVEL_WARN, s, __VA_ARGS__)
#define LOGERROR(s,...) LogErrorASync( __FILE__, __LINE__,LOG_LEVEL_ERROR, s, __VA_ARGS__)
//#define LOG(s, ...) LogError( __FILE__, __LINE__,s, __VA_ARGS__)
#ifdef _DEBUG
#define LOGTRACE(s, ...) LogErrorASync( __FILE__, __LINE__, LOG_LEVEL_DEBUG, s, __VA_ARGS__)
#else
#define LOGTRACE(s, ...) LogErrorASync( __FILE__, __LINE__, LOG_LEVEL_TRACE, s, __VA_ARGS__)
#endif

#define LOGCOMMON(s, ...) LogCommon( s, __VA_ARGS__)
#else
#define LOG(s, args...) LogErrorASync( __FILE__, __LINE__,LOG_LEVEL_DEBUG, s, ##args)
#define LOGWARN(s,args...) LogErrorASync( __FILE__, __LINE__,LOG_LEVEL_WARN, s, ##args)
#define LOGERROR(s,args...) LogErrorASync( __FILE__, __LINE__,LOG_LEVEL_ERROR, s, ##args)
//#define LOG(s, ...) LogError( __FILE__, __LINE__,s, __VA_ARGS__)
#ifdef _DEBUG
#define LOGTRACE(s, args...) LogErrorASync( __FILE__, __LINE__, LOG_LEVEL_DEBUG, s, ##args)
#else
#define LOGTRACE(s, args...) LogErrorASync( __FILE__, __LINE__, LOG_LEVEL_TRACE, s, ##args)
#endif

#define LOGCOMMON(s, args...) LogCommon( s, ##args)
#endif // LIB_WINDOWS

void ReOpenLogFile();
void OpenLog();
void CloseLog();
BOOL IsOpenLog();

//////////////////////////////////////////////////////////////////////////

static inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x -10 + 'A': x + '0';
}

static inline BYTE fromHex(const BYTE &x)
{
	return isdigit(x) ? x-'0' : x-'A'+10;
}

// URL����
inline std::string URLEncode(const std::string &sIn)
{
	std::string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{      
		BYTE buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (BYTE)sIn[ix] ) )
		{      
			buf[0] = sIn[ix];
		}
		//else if ( isspace( (BYTE)sIn[ix] ) ) //�ѿո�����%20����+������
		//{
		//    buf[0] = '+';
		//}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
			buf[2] = toHex( (BYTE)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
};

// URL����
inline std::string URLDecode(const std::string &szToDecode)
{
	std::string result;  
	int hex = 0;  
	for (size_t i = 0; i < szToDecode.length(); ++i)  
	{  
		switch (szToDecode[i])  
		{  
		case '+':  
			result += ' ';  
			break;  
		case '%':  
			if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))  
			{
				std::string hexStr = szToDecode.substr(i + 1, 2);  
				hex = strtol(hexStr.c_str(), 0, 16);
				//��ĸ������[0-9a-zA-Z]��һЩ�������[$-_.+!*'(),] ���Լ�ĳЩ������[$&+,/:;=?@]  
				//���Բ���������ֱ������URL  
				if (!((hex >= 48 && hex <= 57) || //0-9  
					(hex >=97 && hex <= 122) ||   //a-z  
					(hex >=65 && hex <= 90)     //A-Z  
					//һЩ������ż�������[$-_.+!*'(),]  [$&+,/:;=?@]  
					/*|| hex == 0x21 || hex == 0x27 || hex == 0x28 || hex == 0x29 
					|| hex == 0x2a || hex == 0x2d || hex == 0x2e
					|| hex == 0x5f */
					))  
				{  
					result += char(hex);
					i += 2;  
				}  
				else result += '%';  
			}else {  
				result += '%';  
			}  
			break;  
		default:
			result += szToDecode[i];  
			break;  
		}  
	}  
	return result;
}

#ifdef LIB_LINUX
// ȷ��·������.�������򴴽��ļ���
BOOL MakeSureDirectoryPathExists( LPCSTR DirPath );
// ��ȡ�ļ���С
size_t GetFileSize(FILE* pFile);
// �����ǰ��ջ��Ϣ����־
void PrintBackTrace();
// �ź�����
bool BlockSignal();
// �����źŴ�����
typedef void(*signal_handle)(int);
void SetConsoleCtrlHandler(signal_handle handle);
#endif // LIB_LINUX

#ifdef LIB_WINDOWS

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif

struct timezone 
{  
	int  tz_minuteswest; // minutes W of Greenwich  
	int  tz_dsttime;     // type of dst correction
};

// Windows��gettimeofday
int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif // LIB_WINDOWS

// ��ȫ�رյ�ǰ����[����������]
void SafeTerminateProcess();

// UTF8�ַ���ת��ΪUNICODE�ַ���
int utf8tounicode(const char *utf8_buf, wchar_t* unicode_buf, int max_size);
// UNICODE�ַ���ת��ΪUTF8�ַ���
int unicodetoutf8(const wchar_t* unicode_buf, char* utf8_buf,int max_size);

// ��ȡtNow֮�����һ������wWeekDay��wHourʱ��UTCʱ��.(24Сʱ��)
// ����GetNextWeekDayTime(tNow,SYS_WEEK_SUNDAY,2)��ʾ��ȡ��һ������2���ʱ��
__time32_t GetNextWeekDayTime(__time32_t tNow, WORD wWeekDay, WORD wHour);

// ��������������������������.����20110802
int CalcTomorrowYYYYMMDD();
// ��ȡ��ǰ��������·��
void GetModuleFilePath(char* pszOut, int nLen);
// ��ȡ��ǰ��������
void GetModuleFileName(char* pszOut,int nLen);

// ��һ��UTCʱ��ת��Ϊ�ַ�����ʽ��ʱ��(2011-09-02 22:11:02)
// ���� : 0 = (0000-00-00 00:00:00)
const wchar_t* CovertUTC2String(__time32_t tTime, wchar_t* pwzOut, int nOutMaxLen);
const char* CovertUTC2String(__time32_t tTime, char* pszOut, int nOutMaxLen);

//�� ��|��|��|ʱ|��|�� ��ʱ��ת��Ϊʱ��
__time32_t MakeStrTimeToUTC(LPCWSTR lpwzTime);
__time32_t MakeStrTimeToUTC(LPCSTR lpwzTime);

//�� ��|��|ʱ|��|�� ��ʱ�����ݸ��������ת��Ϊʱ��
__time32_t MakeStrTimeToUTC_NoYear(LPCWSTR lpwzTime, UINT nYear);
__time32_t MakeStrTimeToUTC_NoYear(LPCSTR lpwzTime, UINT nYear);

// ����һ��������ַ���
LPCSTR MakeRandomKey(LPSTR pszOut, UINT nOutMaxLen,UINT nNeedLen);
LPCWSTR MakeRandomKey(LPWSTR pszOut, UINT nOutMaxLen,UINT nNeedLen);

// ������(')��˫����(")֮ǰ���ת��б��(\)
// ע��.��Ҫ��ε���.�Ѿ���ӵĻ��������һ��
void AddSlashes(const wchar_t* pwzIn, wchar_t* pwzOut, int nLen);
void AddSlashes(const char* pszIn, char* pszOut, int nLen);

// �� xx|xx|xx ��ʽ�������ɢ��vector
void WINAPI ParseListStr(LPCWSTR pwzList, std::vector< std::vector<int> >& vctOut);
void WINAPI ParseListStr(LPCSTR pszList, std::vector< std::vector<int> >& vctOut);

// ���ַ���ȫ��ת��ΪСд
int StringToLowcase(LPCWSTR lpwzSrc, LPWSTR lpwzOut, int nLen);
int StringToLowcase(LPCSTR lpszSrc, LPSTR lpszOut, int nLen);

//�Ƴ��ַ���ǰ��ո�
void RemoveSpace(LPCWSTR pwzSrc, LPWSTR pwzDst, int nLen);
void RemoveSpace(LPCSTR pszSrc, LPSTR pszDst, int nLen);

// ��ȡ����IP
void GetLocalIP(vector<string>& vctIPList);

// ����Ƿ���debug�汾
bool CheckDebugVersion();
// ����Ƿ�����ʱ��
bool CheckTimezoneZero();
