#pragma once


#include "../Service/Utility.h"
#include <string>
#include "../Global/GlobalServer.h"

#define NAME_DB_IDX			"0"	//�ǳ����ݿ���
#define INVALID_DBQUERY_KEY -1//����Ҫ���������ʱ��keyʹ�ø�ֵ

//���ִ����־
#define SET_QUERY_INFO(t, sql)	int nDQueryKey = INVALID_DBQUERY_KEY;	\
    CDbQueryInfo* pQueryInfo = m_QueryPool.Get();	\
    if (pQueryInfo)	\
                                {	\
                                pQueryInfo->qtType = t;	\
                                nDQueryKey = pQueryInfo->GetIndexInPool();	\
                                pQueryInfo->sSql = sql;	\
                                }	\

#define DEFAULT_SET_QUERY	SET_QUERY_INFO(CDbQueryInfo::qtDefault, szSql)

//struct RESPONSEINFO : public CRecycle
//{
//    bool bSuccessd;
//    DWORD dwKey;
//    CMySQLRecordSet* pResSet;
//};



class CDbQueryInfo : public CRecycle
{
public:
   
    long		    nType;//��������
    DWORD			wParam;
    DWORD			lParam;
    std::string		s;
    DWORD			dwTime;
    std::string		sSql;
    std::string		sParam;

    CDbQueryInfo() {}
    virtual void Init()
    {
        wParam = lParam = 0;
        dwTime = GetTickCount();
        nType = 0;
        sSql.clear();
        sSql.reserve(32);
        s.clear();
        sParam.clear();
    }
};

//typedef void (*PARSERESPONSEFUNC)(CDbQueryInfo*, RESPONSEINFO*);




//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
#include "MSSQLDBSys.h"
//#include "../alloc/kl_allocator.h"
#define MSRESP_OUTPUTPARAM_MAX	4
struct MSRESPONSEINFO : public CRecycle
{
    long                nErrorCode;
    UINT_PTR            nKey;
    long                nOutputParam[MSRESP_OUTPUTPARAM_MAX];
    CMSSQLRecordSet*    pResSet;

    virtual void Init()
    {
        nErrorCode = 0;
        nKey = 0;
        memset( nOutputParam, 0, sizeof(nOutputParam) );
        pResSet = 0;
    }
};
typedef CWQueue<MSRESPONSEINFO*> QueueMSResp;

class CMSDbQueryInfo : public CRecycle
{
public:

    long		    nType;//��������
    //CMSSQLCommand*  pCmd;
    char            m_strSPName[64];
    DWORD			dwTime;

    UINT_PTR    wParam;
    UINT_PTR    lParam;
    UINT_PTR    kParam;
	UINT_PTR    xParam;
    UINT_PTR    yParam;
	UINT_PTR	zParam;
	_variant_t	varOut;

    CMSDbQueryInfo() { Init(); }
    virtual void Init()
    {
        nType = 0;
        //pCmd = 0;
        dwTime = GetTickCount();
        memset( m_strSPName, 0, sizeof(m_strSPName) );
        wParam = 0;
        lParam = 0;
		kParam = 0;
        xParam = 0;
        yParam = 0;
		zParam = 0;
    }
};

typedef void (*MSPARSERESPONSEFUNC)(CMSDbQueryInfo*, MSRESPONSEINFO*);

template<int SIZE>
class CMSDbManagerBase
{
public:

	CMSDbManagerBase()  : m_QueryPool("CMSDbQueryInfo"),m_ResponsePool("MSRESPONSEINFO")
    {
        m_nLastIndex = 0;
        m_nDbCount = 0;
        m_nHashNum = 0;
        memset(m_ParserFuncs, 0, sizeof(m_ParserFuncs));
        memset(m_ParserFail, 0, sizeof(m_ParserFail));
		memset(m_szSQLName, 0, sizeof(m_szSQLName));
    }
    //ST_INSTANCE(CDbManagerBase);
    bool Init(const std::string& sDbConfigFile);//��ʼ��

    void ParseResponse( long nErrorCode, UINT_PTR dwKey, long* pRetValue, long nRetCount, CMSSQLRecordSet* t_ResSet);//����db��������
    void OnDataReceive( /*CMSDbManagerBase* pThis*/ );//����db��������,���¼�����,�����߳��е���
    void CheckQuery();
    void ProcessResult(MSRESPONSEINFO* pResponseInfo);

public:
    //ST_STRUCTOR_NOCON(CDbManagerBase);
    bool LoadConfig(const std::string& sDbConfigFile);//��ȡ�����ļ�
    bool ConnectDbs();	//�������ݿ�
    void SetConnectInfo(MSCONNECT_INFO& ConnectInfo, CStringVct& vctResult);
#define MSDBSQL_NAME_MAXLEN	128
    void Register(WORD wType, MSPARSERESPONSEFUNC pFunc, LPCSTR pszSQLName )
    {
        if (wType >= SIZE)
        {
			LOG("CMSDbManagerBase::Register Invalid Type=%d>=%d Error!", wType, SIZE );
            return;
        }
        m_ParserFuncs[wType] = pFunc;
		strncpy_s( m_szSQLName[wType], pszSQLName, MSDBSQL_NAME_MAXLEN-1 );
		m_szSQLName[wType][MSDBSQL_NAME_MAXLEN-1] = '\0';
    }
    virtual void SetEvent(){}

    MSRESPONSEINFO* ApplyRespInfo()
    { 
		MSRESPONSEINFO* pRespInfo = m_ResponsePool.ApplyObj();
        //LOG( "ApplyRespInfo" );
		/*Removed by Waj at [10/8/2010 ]
		LOGASync( "ApplyRespInfo=0x%08x", pRespInfo );
		*/
        return pRespInfo; 
    }
    void ReclaimRespInfo( MSRESPONSEINFO* pRespInfo )
    { 
        /*Removed by Waj at [10/8/2010 ]
        LOGASync( "ReclaimRespInfo=0x%08x", pRespInfo );
        */
        m_ResponsePool.ReclaimObj( pRespInfo ); 
    }

    CMSDbQueryInfo* ApplyQueryInfo()
    { 
		CMSDbQueryInfo* pQueryInfo = m_QueryPool.ApplyObj();
		/*Removed by Waj at [10/8/2010 ]*/
		if( pQueryInfo == NULL)
			LOG( "DB=0x%08x ApplyQueryInfo Error=%d", this, GetLastError() );

        return pQueryInfo; 
    }
    void ReclaimQueryInfo( CMSDbQueryInfo* pQueryInfo )
    {
		/*Removed by Waj at [10/8/2010 ]
		LOGASync( "DB=0x%08x ReclaimQueryInfo=0x%08x", this, pQueryInfo );
		*/
        m_QueryPool.ReclaimObj( pQueryInfo ); 
    }
	void	WaitForAllDBReqIdle();
	void	WaitForAllDBRespIdle();
	size_t	GetReqInQueue();
	size_t	GetCmdInQueue( size_t ay[32], size_t ayLen[32] );
	void	ResetClient( size_t nClientID );
	CMSSQLRecordSet* ApplyRecSet()
	{
		return m_DBSys.ApplyRecSet();
	}
	void ReclaimRecSet( CMSSQLRecordSet* pRecSet )
	{
		m_DBSys.ReclaimRecSet(pRecSet);
	}
	void PingMySQL(UINT nHash);
protected:
    std::vector<MSCONNECT_INFO>     m_DBSeverList;//���ݿ����Ӳ����б�
    CMSSQLDBSystem                  m_DBSys;

    CPool<CMSDbQueryInfo,1024>           m_QueryPool;

    MSPARSERESPONSEFUNC	            m_ParserFuncs[SIZE];//������
    bool				            m_ParserFail[SIZE];//ʧ��ʱ�Ƿ���ô�����
	char							m_szSQLName[SIZE][MSDBSQL_NAME_MAXLEN];

    CPool<MSRESPONSEINFO,1024>           m_ResponsePool;
    //CLock		                    m_ResponseLock;		//mysql���������
	QueueMSResp	m_qResponse;
    //CTmpChrInfos	m_TmpChrInfos;
    size_t				                m_nLastIndex;//���һ�α�����������
    size_t				                m_nDbCount;//�洢��ɫ��Ϣ�����ݿ�����
    size_t				                m_nHashNum;//ʹ��hash�㷨ʱ,�����ʺŵ�λ��
};

template<int SIZE>
void CMSDbManagerBase<SIZE>::ResetClient( size_t nClientID )
{
	m_DBSys.ResetClient( nClientID );
}


//��ʼ��
template<int SIZE>
bool CMSDbManagerBase<SIZE>::Init(const std::string& sDbConfigFile)
{
    if (!LoadConfig(sDbConfigFile))
        return false;

    if (!ConnectDbs())
        return false;
    return true;
}

template<int SIZE>
void CMSDbManagerBase<SIZE>::SetConnectInfo(MSCONNECT_INFO& ConnectInfo, CStringVct& vctResult)
{
    _snprintf_s(ConnectInfo.m_hostName, sizeof(ConnectInfo.m_hostName)-1, "%s", vctResult[0].c_str());
    ConnectInfo.m_hostPort = atoi(vctResult[1].c_str());
    _snprintf_s(ConnectInfo.m_dbName, sizeof(ConnectInfo.m_dbName)-1, "%s", vctResult[2].c_str());
    _snprintf_s(ConnectInfo.m_userName, sizeof(ConnectInfo.m_userName)-1, "%s", vctResult[3].c_str());
    if (vctResult.size() > 4)
        _snprintf_s(ConnectInfo.m_password, sizeof(ConnectInfo.m_password)-1, "%s", vctResult[4].c_str());
    else ConnectInfo.m_password[0] = 0;
}

//��ȡ�����ļ�
template<int SIZE>
bool CMSDbManagerBase<SIZE>::LoadConfig(const std::string& sDbConfigFile)
{
    std::string sSection = "chrdb";
    char szBuf[1024] = {0};
    char szKey[256] = {0};
    CStringVct vctResult;
    CONNECT_INFO ConnectInfo;

    m_nDbCount = GetPrivateProfileIntA(sSection.c_str(),"dbcount",0,sDbConfigFile.c_str());
    if (m_nDbCount <= 0)
    {
        LOG( "���ݿ�������ó���");
        return false;
    }
    m_nHashNum = GetPrivateProfileIntA(sSection.c_str(),"hashnum",0,sDbConfigFile.c_str());
    if (m_nHashNum<=0 || m_nHashNum>2)
    {
        LOG( "��ϣλ�����ó���");
        return false;
    }

    for (int i=0; i<m_nDbCount; ++i)
    {
        //���ø�ʽ:	db��� = ������ip �˿� ���ݿ� �ʺ� ����
        sprintf_s(szKey, _countof(szKey), "db%d", i);
        if((GetPrivateProfileStringA(sSection.c_str(),szKey,"",szBuf,sizeof(szBuf),sDbConfigFile.c_str())) == 0)
        {
            LOG( DB_CONFIG_NOSET, i);
            return false;
        }
        StrDelimiter(szBuf, "\t ", vctResult);
        if (vctResult.size() < 4)
        {
            LOG( DB_CONFIG_SET_ERR, i);
            return false;
        }

        //����
        SetConnectInfo(ConnectInfo, vctResult);

        //�����б�
        m_DBSeverList.push_back(ConnectInfo);
    }

    //��ȡ�ǳ����ݿ�
    if((GetPrivateProfileStringA(sSection.c_str(),"nickname","",szBuf,sizeof(szBuf),sDbConfigFile.c_str())) == 0)
    {
        LOG( "�ǳ����ݿ�δ����");
        return false;
    }
    StrDelimiter(szBuf, "\t ", vctResult);
    if (vctResult.size() < 4)
    {
        LOG( "�ǳ����ݿ����ø�ʽ����");
        return false;
    }
    SetConnectInfo(ConnectInfo, vctResult);
    m_DBSeverList.push_back(ConnectInfo);

    return true;
}

template<int SIZE>
bool CMSDbManagerBase<SIZE>::ConnectDbs()
{
    //���ݿ�����
    std::cout << BEGIN_CONNECT_DB << std::endl;
    m_DBSys.SetMysqlServerListInfo(m_DBSeverList);
    if(!m_DBSys.Startup())
    {
        std::vector<int> vctFailed = m_DBSys.GetFailedConnVec();
        for (int i=0; i< (int)vctFailed.size(); ++i)
        {
            LOG( CONNECT_DB_FAIL, vctFailed[i]);
        }
        return  false;
    }
    std::cout << END_CONNECT_DB << std::endl;
    return true;
}

//����db��������,��CallBack�е���
template<int SIZE>
void CMSDbManagerBase<SIZE>::ParseResponse( long nErrorCode, UINT_PTR dwKey, long* pRetValue, long nRetCount, CMSSQLRecordSet* pResSet)
{
    if (dwKey == INVALID_DBQUERY_KEY)
    {
        if( nErrorCode == 0 )	/*if ( nErrorCode != 0)*///waj
        {
            LOG( "CDbManagerBase::ParseResponse���ݿ����ʧ��");
        }
        if (pResSet)
        {
            m_DBSys.ReclaimRecSet( pResSet );
        }
        return;
    }
    MSRESPONSEINFO* pResponseInfo = ApplyRespInfo();
    pResponseInfo->nErrorCode = nErrorCode;
    pResponseInfo->nKey = dwKey;
    pResponseInfo->pResSet = pResSet;
	for( long i = 0; i<nRetCount&&i<MSRESP_OUTPUTPARAM_MAX; i++)
		pResponseInfo->nOutputParam[i] = pRetValue[i];
    BOOL bMustSetEvent = FALSE;//�Ƿ���Ҫ�����¼�
    m_qResponse.Lock();
    if (m_qResponse.empty())
    {
        bMustSetEvent = TRUE;
    }
    m_qResponse.push_back(pResponseInfo);
    m_qResponse.UnLock();
    if (bMustSetEvent)
    {
        SetEvent();
        //g_Application.SetEvent(ei::eiMysqlResponse);
    }
}

//����db��������,���¼�����,�����߳��е���
template<int SIZE>
void CMSDbManagerBase<SIZE>::OnDataReceive( /*CMSDbManagerBase<SIZE>* pThis*/ )
{
    //LOG_DECLARE_BEGIN;
    MSRESPONSEINFO* pResponseInfo = 0;
    //CDbManager* pThis = GetInstance();
    for(;;)
    {
        m_qResponse.Lock();
        if ( m_qResponse.empty())
        {
            m_qResponse.UnLock();
            break;
        }
        pResponseInfo = ( m_qResponse.pop_front());
        m_qResponse.UnLock();
        //����
        TRY_BEGIN_EXCEP
            ProcessResult(pResponseInfo);
        TRY_END_EXCEP("CDbManager::OnDataReceive()");
		if (pResponseInfo->pResSet)
		{
			m_DBSys.ReclaimRecSet( pResponseInfo->pResSet );
			pResponseInfo->pResSet = NULL;
		}
        ReclaimRespInfo( pResponseInfo );
    }
    //LOG_END(LOG_HANDLE_TIME, "CDbManager::OnDataReceive()");
}

template<int SIZE>
void CMSDbManagerBase<SIZE>::ProcessResult( MSRESPONSEINFO* pResponseInfo )
{
    CMSDbQueryInfo* pQueryInfo = (CMSDbQueryInfo*)(pResponseInfo->nKey);
    
    if( !pResponseInfo->nErrorCode )//���ݿ����ʧ��
    {
		if( pQueryInfo->nType != 0 )
			LOG( "CMSDbManagerBase::ProcessResult ���ݿ����ʧ��,����%d, �洢����:%s, Error=%d",  pQueryInfo->nType, pQueryInfo->m_strSPName, pResponseInfo->nErrorCode );
        if (!m_ParserFail[pQueryInfo->nType])//ʧ��ʱ������
        {
            //ReclaimQueryInfo( pQueryInfo );
        }
    }
    if (pQueryInfo)
    {
        if( m_ParserFuncs[pQueryInfo->nType] )
        {
            m_ParserFuncs[pQueryInfo->nType]( pQueryInfo, pResponseInfo );
        }
		else if( pQueryInfo->nType != 0 )
			LOG("CMSDbManagerBase::ProcessResult Invalid QueryType=%d, SP=%s Error!",
					pQueryInfo->nType, pQueryInfo->m_strSPName );
        ReclaimQueryInfo(pQueryInfo);
    }
//     if (pResponseInfo->pResSet)
//     {
//         m_DBSys.ReclaimRecSet( pResponseInfo->pResSet );
// 		pResponseInfo->pResSet = NULL;
//     }
//    ReclaimRespInfo( pResponseInfo );
}

template<int SIZE>
size_t CMSDbManagerBase<SIZE>::GetReqInQueue()
{
	return m_DBSys.GetAllCmdInQueue();
}
template<int SIZE>
size_t CMSDbManagerBase<SIZE>::GetCmdInQueue( size_t ay[32], size_t ayLen[32] )
{
	return m_DBSys.GetCmdInQueue( ay, ayLen );
}
template<int SIZE>
void CMSDbManagerBase<SIZE>::WaitForAllDBReqIdle()
{	//�����ȴ����е����ݿ��߳���ɲ�����ϵͳ�ر�ʱʹ�ø÷���
	while( m_DBSys.IsAllDBClientIdle() == FALSE )
	{
//		LOG("MSDbManagerBase Wait For All Client Idle...");
		Sleep(100);
	}
	LOG("MSDbManagerBase WaitForAllDBReqIdle OK!" );
}
template<int SIZE>
void CMSDbManagerBase<SIZE>::WaitForAllDBRespIdle()
{
	int nRespLeft = 1;
	while( nRespLeft > 0 )
	{
		m_qResponse.Lock();
		nRespLeft = m_qResponse.size();
		m_qResponse.UnLock();
		if( nRespLeft > 0 )
			OnDataReceive();
	}
	LOG("MSDbManagerBase WaitForAllDBRespIdle OK!" );
}

template<int SIZE>
void CMSDbManagerBase<SIZE>::PingMySQL(UINT nHash)
{
	if (m_DBSys.GetDBType() != emDatabaseType_MySQL)
	{
		return;
	}

	CMSSQLCommand* pCmd = m_DBSys.ApplyCmd();
	CMSDbQueryInfo*  pQueryInfo = ApplyQueryInfo();
	if (pCmd == NULL || pQueryInfo == NULL)
	{
		LOG("Apply Object Error : %p.%p",pCmd,pQueryInfo);
		if (pCmd)
		{
			m_DBSys.ReclaimCmd(pCmd);
		}
		if (pQueryInfo)
		{
			ReclaimQueryInfo(pQueryInfo);
		}
		return;
	}

	pCmd->SetCommandText( L"SHOW TABLES;" );
	pCmd->SetCommandType(adCmdText);
	//pCmd->AddParameter( "Return", adInteger, adParamReturnValue, sizeof(int));

	strncpy_s( pQueryInfo->m_strSPName,"SHOW TABLES;", 63  );
	pQueryInfo->nType = 0;
	m_DBSys.AddSQLRequest( (DWORD)pQueryInfo, nHash , pCmd, 0 );
}

__time32_t VariantTimeToTime_t( _variant_t varTime );