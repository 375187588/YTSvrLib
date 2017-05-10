#pragma once

#pragma pack(push,1)

#pragma warning(disable : 4355)

#define UTF8SQL
//////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define DBTCP_DEF_FLAG		0xCCCCCC01
#define DB_PROCNAME_MAXLEN	64		// �洢��������
#ifdef UTF8SQL
#define DB_SQL_MAXLEN		4096		// SQL
#else
#define DB_SQL_MAXLEN		2048		// SQL
#endif // UTF8SQL

//��Ϣ����
#define DBMSG_KEEPALIVE			0x0000
#define DBMSG_SVR_LOGIN			0x0001
#define DBMSG_EXEC_SQL			0x0002
#define DBMSG_QUERY				0x0003

//TCPͷ
typedef struct SDBMsgHead
{
	SDBMsgHead(WORD wType, DWORD dwLen) : m_dwTcpFlag(DBTCP_DEF_FLAG), m_wMsgType(wType), m_dwLen(dwLen) {}
	void Init(WORD wType, DWORD dwLen){
		m_dwTcpFlag = DBTCP_DEF_FLAG;
		m_dwLen = dwLen;
		m_wMsgType = wType;
	}
	DWORD	m_dwTcpFlag;			// ��Ϣ��־
	DWORD	m_dwLen;				// ��Ϣ�ܳ��ȣ�������Ϣͷ
	WORD	m_wMsgType;			// ��Ϣ����
} SDBMsgHead, * LPSDBMsgHead;

typedef int SQLKEY;
#define SQLKEYFORMAT "%d"

struct SDBMsgExecSql : public SDBMsgHead
{
	SDBMsgExecSql() : SDBMsgHead(DBMSG_EXEC_SQL, sizeof(*this))	{}
	void Init() {
		SDBMsgHead::Init( DBMSG_EXEC_SQL, sizeof(*this) );
		m_nKey = 0;
		m_nKey2 = 0;
		m_nKey3 = 0;
		m_bForceWrite = FALSE;
		m_szProcName[0] = '\0';
	}

	BOOL	m_bForceWrite;// ���ֵ������ʶ����洢�Ƿ�����ִ�ж������뻺��,Ĭ��Ϊ��(���뻺��)
	SQLKEY	m_nKey;
	SQLKEY	m_nKey2;
	SQLKEY	m_nKey3;
	char	m_szProcName[DB_PROCNAME_MAXLEN];
#ifdef UTF8SQL
	LPCSTR	GetSQL()
	{
		return (LPCSTR) (this + 1);
	}
#else
	LPCWSTR	GetSQL()
	{
		return (LPCWSTR) (this + 1);
	}
#endif // UTF8SQL
};
typedef SDBMsgExecSql*  LPSDBMsgExecSql;

struct sDBMsgQuery : public SDBMsgHead
{
	sDBMsgQuery() : SDBMsgHead(DBMSG_QUERY, sizeof(sDBMsgQuery)) {}

	DWORD m_dwQueueLen;
};


#pragma pack(pop)


