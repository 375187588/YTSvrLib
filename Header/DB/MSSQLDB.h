#pragma once


#include <string>
#ifdef _WIN64
#import "..\ado64\msado15.dll" no_namespace rename ("EOF", "adoEOF")
#else
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename ("EOF", "adoEOF")
#endif
#include "../Service/Utility.h"

class CCOM
{
public:
    CCOM(){ CoInitialize(NULL);    }
    ~CCOM(){ CoUninitialize();  }
};


typedef enum
{
	emDatabaseType_Null,
	emDatabaseType_MSSQL,
	emDatabaseType_MySQL,
	emDatabaseType_Oracle,
	emDatabaseType_Access,
	emDatabaseType_SQLite,
}EM_DATABASE_TYPE;

class CMSSQLConnection;

/**
@brief ��װADO��_CommandPtr������Ҫ�������CMSSQLConnectionִ�д洢����
*
* CMSSQLCommand��װ��ADO�е�Command���󣬲����ɶ���߳�ͬʱ����������ʵ��
* ��Ҫ�������CMSSQLConnectionִ�д洢����
* ����CMSSQLCommandʱ��Ҫ����CMSSQLConnection�Ķ���ָ��
* ���״ε���AddParameterʱ��Ҫȷ�������б������
* �ɵ��÷����𲶻�_com_error �쳣
*/
void inline EfficacyResult(HRESULT hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

class CMSSQLCommand:public CRecycle
{
    friend class CMSSQLConnection;
public:
    CMSSQLCommand()
    {
        m_pCommand.CreateInstance(__uuidof(Command));
    }

    ~CMSSQLCommand()
    {
		m_pCommand.Release();
		m_pCommand = NULL;
    }

    virtual void Init(){ ClearParameters(); }

    ///����Command Type
    void SetCommandType(enum CommandTypeEnum Type) { m_pCommand->CommandType = Type; }
	enum CommandTypeEnum GetCommandType() { return m_pCommand->CommandType; };

    ///����Command Text
    void SetCommandText(const char* pszText) { m_pCommand->CommandText = pszText; }
	void SetCommandText(LPCWSTR pwzText) { m_pCommand->CommandText = pwzText; }

    void Refresh(){ m_pCommand->Parameters->Refresh(); }

    ///����һ������
    void AddParameter(const char* Name, enum DataTypeEnum Type, enum ParameterDirectionEnum Direction,
        long Size, const _variant_t& Value = vtMissing)
    {
        try
        {
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(Name, Type, Direction, Size, Value));
			if( Direction == adParamOutput || Direction == adParamInputOutput )
				m_nOutputParam++;
        }
		catch(_com_error& e)
		{
			_bstr_t   bs   =   _bstr_t("DBCmd=") + m_pCommand->CommandText + _bstr_t(" Name=") + _bstr_t(Name)
							+ _bstr_t(" Error=")   +   _bstr_t(e.Error())   +   _bstr_t(" Msg=")     
							+   _bstr_t(e.ErrorMessage())   +   _bstr_t(" Description:   ")     
							+   _bstr_t(e.Description()); 
			LOG( (char*)bs );
		}
    }
	void AddParameterBinary( const char* Name,const char* pData, int nDataLen )
	{
		try
		{
			_variant_t		varBLOB;
			SAFEARRAY	*psa;
			SAFEARRAYBOUND	rgsabound[1];
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = nDataLen;
			psa = SafeArrayCreate(VT_UI1, 1, rgsabound);                      ///����SAFEARRAY����
			for (long i = 0; i <nDataLen; i++)
				SafeArrayPutElement (psa, &i, (void*)(pData+i));                         ///��pBufָ��Ķ��������ݱ��浽SAFEARRAY����psa��
			varBLOB.vt = VT_ARRAY | VT_UI1;                                   ///��varBLOB����������ΪBYTE���͵�����
			varBLOB.parray = psa;   
			AddParameter( Name, adVarBinary, adParamInput, nDataLen+1, varBLOB );
		}
		catch(_com_error& e)
		{
			_bstr_t   bs   =   _bstr_t("DBCmd=") + m_pCommand->CommandText + _bstr_t(" Name=") + _bstr_t(Name)
				+ _bstr_t(" Error=")   +   _bstr_t(e.Error())   +   _bstr_t(" Msg=")     
				+   _bstr_t(e.ErrorMessage())   +   _bstr_t(" Description:   ")     
				+   _bstr_t(e.Description()); 
			LOG( (char*)bs );
		}
	}
    ///��ȡĳ�������ķ���ֵ
    _variant_t GetItemValue(const char* Name) 
    { 
        return m_pCommand->Parameters->GetItem(Name)->GetValue(); 
    }

    _variant_t GetItemValue( short nIdx )
    {
        return m_pCommand->Parameters->GetItem(short(nIdx))->Value;
    }
	enum ParameterDirectionEnum GetItemDirection(short nIdx)
	{
		return m_pCommand->Parameters->GetItem(short(nIdx))->Direction;
	}
	UINT GetItemCount()
	{
		return m_pCommand->Parameters->GetCount();
	}
    ///������в�����Ŀǰδ�ṩ
    void ClearParameters()
    {
		m_nOutputParam = 0;
        for (long i = m_pCommand->Parameters->Count - 1; i >= 0; i--)
            m_pCommand->Parameters->Delete(i);
		m_pCommand->Parameters->Release();
		SetCommandText("");
    }

    ///ɾ��һ������
    void DeleteParameter(const char* Name)  
    { 
        m_pCommand->Parameters->Delete(Name); 
    }
	long GetOutputParamCount() { return m_nOutputParam; }

private:
    _CommandPtr m_pCommand;
	long m_nOutputParam;
};

/**
@brief ��װADO��_RecordsetPtr����
*
*	��װADO��_RecordsetPtr���󣬲����ɶ���߳�ͬʱ����������ʵ��
*  �ɵ��÷����𲶻�_com_error �쳣
*/
class CMSSQLRecordSet:public CRecycle
{
    friend class CMSSQLConnection;
public:
    CMSSQLRecordSet()
    { 
        m_pRecordSet = NULL;//m_pRecordSet.CreateInstance(__uuidof(Recordset));
    }

    ~CMSSQLRecordSet(){ /*m_pRecordSet.Release();*/ };

    virtual void Init()
    {
        //DeleteAll();
        //Close(); 
		//m_nOutputParams = 0;
    }

    ///�ƶ�����¼���ײ�
    void MoveFirst() { if (m_pRecordSet)  m_pRecordSet->MoveFirst(); }

    ///�ƶ�����¼����һ��
    void MoveNext() { if (m_pRecordSet) m_pRecordSet->MoveNext(); }

    ///�ƶ�����¼����һ��
    void MovePrevious() { if (m_pRecordSet)  m_pRecordSet->MovePrevious(); }

    ///�ƶ�����¼��β��
    void MoveLast() { if (m_pRecordSet) m_pRecordSet->MoveLast(); }

    ///�Ƿ�Խ����¼��ĩβ
    BOOL IsEof()
    { 
        if (m_pRecordSet) 
            return m_pRecordSet->adoEOF; 
        else 
            return FALSE;
    }

    void Close()
    { 
        if (m_pRecordSet==NULL) return ;
        if (m_pRecordSet->GetState()!=adStateOpen)
		{
#ifdef _DEBUG
			LOG("RecordSet=0x%08x(0x%08x) State=%d donot Closed!", this, m_pRecordSet, m_pRecordSet->GetState());
			try
			{
				switch( m_pRecordSet->GetState() )
				{
				case adStateClosed:
					{
						//m_pRecordSet->Close();
						break;
					}
				case adStateOpen:
				case adStateConnecting:
				case adStateExecuting:
				case adStateFetching:
					{
				//		m_pRecordSet->Close();
						break;
					}
				}
			}
			catch(_com_error& e)
			{
				_bstr_t   bs   =   _bstr_t("RecordSet Close Error=") + _bstr_t(e.Error())   +   _bstr_t(" Msg=")     
					+   _bstr_t(e.ErrorMessage())   +   _bstr_t(" Description:   ")     
					+   _bstr_t(e.Description()); 
				LOG( (char*)bs );
			}
#endif
			return;
		}
        if ( FAILED(m_pRecordSet->Close()) )
        {
            LOG("RecordSet=0x%08x(0x%08x) Close() error!", this, m_pRecordSet );
            //_com_issue_error(hResult);
        }
// #ifdef _DEBUG
// 		else
// 			LOG("RecordSet=0x%08x(0x%08x) Closed.", this, m_pRecordSet );
// #endif
    }

    void DeleteAll()
    {
        if (m_pRecordSet==NULL) return ;
        if (m_pRecordSet->GetState()==adStateClosed) return ;
        m_pRecordSet->Delete(adAffectAll); 
    }

    ///�Ƿ�Խ����¼���ײ�
    BOOL IsBof() 
    { 
        if (m_pRecordSet) 
            return m_pRecordSet->BOF; 
        else
            return FALSE;
    }

    ///�õ���¼����
    DWORD GetRecordCount() 
    { 
        if (m_pRecordSet) 
        {
            return (DWORD)m_pRecordSet->GetRecordCount(); 
        }
        else
        {
            return 0;
        }
    }

    long GetState()
    {
        return m_pRecordSet->GetState();
    }

    ///��ȡĳһ�ֶε�ֵ
    _variant_t GetFieldValue(const char* pszFieldName)
    {
        _variant_t vtResult;
		DataTypeEnum emType;
        if (m_pRecordSet) 
		{
			vtResult = m_pRecordSet->Fields->GetItem(pszFieldName)->Value;
			emType = m_pRecordSet->Fields->GetItem(pszFieldName)->GetType();
		}
        else
		{
            vtResult.vt = VT_NULL;
		}

		if( vtResult.vt == VT_NULL )
		{
			if (emType == adInteger || emType == adBigInt)
			{
				vtResult = 0;
			}
			else if (emType == adVarWChar)
			{
				vtResult = L"";
			}
			else if (emType == adVarChar)
			{
				vtResult = "";
			}
			else if (emType == adLongVarWChar)
			{
				vtResult = L"";
			}
			else if (emType == adLongVarChar)
			{
				vtResult = "";
			}
		}
			
        return vtResult;
    }

    ///�õ���¼�����ֶεĸ���
    DWORD GetFieldCount() 
    {
        if (m_pRecordSet) 
            return m_pRecordSet->Fields->Count; 
        else
            return 0;
    }
	_bstr_t GetFieldName(long nIndex)
	{
		_bstr_t bstrName;
		if (m_pRecordSet) 
		{
			bstrName = m_pRecordSet->Fields->GetItem(nIndex)->GetName();
		}
		else
		{
			bstrName = _bstr_t("");
		}

		return bstrName;
	}
	DataTypeEnum GetFieldType(long nIndex)
	{
		DataTypeEnum emType;
		if (m_pRecordSet) 
		{
			emType = m_pRecordSet->Fields->GetItem(nIndex)->GetType();
		}
		else
		{
			emType = adVarChar;
		}

		return emType;
	}
    _variant_t GetFieldValue( long nIndex )
    {
		_variant_t vtResult;
		DataTypeEnum emType;
		if (m_pRecordSet) 
		{
			vtResult = m_pRecordSet->Fields->GetItem(nIndex)->Value;
			emType = m_pRecordSet->Fields->GetItem(nIndex)->GetType();
		}
		else
		{
			vtResult.vt = VT_NULL;
		}

		if( vtResult.vt == VT_NULL )
		{
			if (emType == adInteger || emType == adBigInt)
			{
				vtResult = 0;
			}
			else if (emType == adVarWChar)
			{
				vtResult = L"";
			}
			else if (emType == adVarChar)
			{
				vtResult = "";
			}
			else if (emType == adLongVarWChar)
			{
				vtResult = L"";
			}
			else if (emType == adLongVarChar)
			{
				vtResult = "";
			}
			else
			{
				vtResult = "";
			}
		}

		return vtResult;
    }
	long GetFieldValueSize( long nIndex )
	{
		return (long)m_pRecordSet->Fields->GetItem( nIndex )->ActualSize;
	}
	_variant_t GetFieldValueWStr( long nIndex )
	{
		_variant_t varRet = m_pRecordSet->Fields->GetItem( nIndex )->Value;
		if( varRet.vt == VT_NULL )
			varRet = L"";
		return varRet;
	}
	_variant_t GetFieldValueWStr(const char* lpszFiledName)
	{
		_variant_t varRet = m_pRecordSet->Fields->GetItem( lpszFiledName )->Value;
		if( varRet.vt == VT_NULL )
			varRet = L"";
		return varRet;
	}
	_variant_t GetFieldValueStr( long nIndex )
	{
		_variant_t varRet = m_pRecordSet->Fields->GetItem( nIndex )->Value;
		if( varRet.vt == VT_NULL )
			varRet = "";
		return varRet;
	}
	_variant_t GetFieldValueBinary( long nIdx )
	{
		_variant_t varBLOB;
		long lDataSize = (long)m_pRecordSet->Fields->GetItem(nIdx)->ActualSize;///�õ����ݵĳ���
		if(lDataSize > 0)
		{
			varBLOB = m_pRecordSet->Fields->GetItem(nIdx)->Value;
		}
		return varBLOB;
	}
	_variant_t GetFieldValueBinary( const char* pszName )
	{
		_variant_t varBLOB;
		long lDataSize = (long)m_pRecordSet->Fields->GetItem(pszName)->ActualSize;///�õ����ݵĳ���
		if(lDataSize > 0)
		{
			varBLOB = m_pRecordSet->Fields->GetItem(pszName)->Value;
		}
		return varBLOB;
	}
	long GetFieldValueBinary( long nIdx, char* pszBuf, int nBufSize )
	{
		_variant_t varBLOB = GetFieldValueBinary( nIdx );
		int nDataLen = 0;
		if(varBLOB.vt == (VT_ARRAY | VT_UI1))                                 ///�ж����������Ƿ���ȷ
		{
			nDataLen = varBLOB.parray->rgsabound[0].cElements;
			if( nDataLen > nBufSize )
				nDataLen = nBufSize;
			char *pBuf = NULL;
			SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);              ///�õ�ָ�����ݵ�ָ��
			memcpy(	pszBuf, pBuf, nDataLen );
			SafeArrayUnaccessData (varBLOB.parray);
		}
		return nDataLen;
	}
	long GetFieldValueBinary( const char* pszKeyName, char* pszBuf, int nBufSize )
	{
		_variant_t varBLOB = GetFieldValueBinary( pszKeyName );
		int nDataLen = 0;
		if(varBLOB.vt == (VT_ARRAY | VT_UI1))                                 ///�ж����������Ƿ���ȷ
		{
			nDataLen = varBLOB.parray->rgsabound[0].cElements;
			if( nDataLen > nBufSize )
				nDataLen = nBufSize;
			char *pBuf = NULL;
			SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);              ///�õ�ָ�����ݵ�ָ��
			memcpy(	pszBuf, pBuf, nDataLen );
			SafeArrayUnaccessData (varBLOB.parray);
		}
		return nDataLen;
	}
	void NextRecordset( CMSSQLRecordSet* pRsSet )
	{
		//_variant_t   vta=0; 
		m_pRecordSet = NULL;
		if( pRsSet && pRsSet->m_pRecordSet )
		{
			long lngRec = 0;
			m_pRecordSet = pRsSet->m_pRecordSet->NextRecordset((VARIANT *)lngRec);
		}
	}
	void NextRecordset()
	{
		long lngRec = 0;
		m_pRecordSet = m_pRecordSet->NextRecordset((VARIANT *)lngRec);
	}
// 	UINT GetOutputParamCount() { return m_nOutputParams; }
// 	BOOL	AddOutputParam( int nOutput )
// 	{
// 		if( m_nOutputParams >= MSSQL_OUTPUT_PARAM_MAX )
// 			return FALSE;
// 		m_ayOutputParams[m_nOutputParams] = nOutput;
// 		m_nOutputParams++;
// 		return TRUE;
// 	}
// 	int	GetOutputParam( long nIdx )
// 	{
// 		if( nIdx < m_nOutputParams )
// 			return m_ayOutputParams[nIdx];
// 		return 0;
// 	}
private:
    _RecordsetPtr m_pRecordSet;

// 	int m_ayOutputParams[MSSQL_OUTPUT_PARAM_MAX];
// 	long m_nOutputParams;
};


/**
@brief ��װADO��_ConnectionPtr����
*
*  ��װADO��_ConnectionPtr���󣬲����ɶ���߳�ͬʱ����������ʵ��
*  �ڽ���_com_error�쳣���񣬵�����FALSEʱGetLastErrorInfo�ɻ�ô�������
*/

class CMSSQLConnection
{
    friend class CMSSQLCommand;
    friend class CMSSQLRecordSet;
public:
    CMSSQLConnection(void);
    ~CMSSQLConnection(void);

    ///���������ַ���
    void SetConnectionString(const char* pszConnectionString) { m_strConnectionString = pszConnectionString; }

    ///������
    BOOL Connect(const char* pszConnectionString);

    ///������
    BOOL Connect();

    ///�ر�����
    void Close();

    ///ִ��һ��CMSSQLCommand���󣬲����ؼ�¼��
    BOOL Execute( CMSSQLCommand* pCmd, CMSSQLRecordSet* pSet);

    ///ִ��һ��CMSSQLCommand���󣬲����ؼ�¼��
    //BOOL Execute( CMSSQLRecordSet& pSet);

    ///ִ��һ��CMSSQLCommand���󣬲����ؼ�¼��
    BOOL Execute( CMSSQLCommand* pCmd );

    ///ִ��һ��SQL��䣬�����ؼ�¼��
    BOOL Execute(char* pszSQL, CMSSQLRecordSet& pSet);

    ///ִ��һ��SQL��䣬�����ؼ�¼��
    BOOL Execute(char* pszSQL);

    ///�Ƿ�������
    BOOL IsConnected();

    ///��ȡ��һ�������
    DWORD GetLastError() { return m_dwErrCode; }

    ///��ȡ��һ����������
    const char* GetLastErrorInfo() { return m_strErrInfo.c_str(); }

    //CMSSQLCommand   m_Command;

protected:

private:
    //driver=sql Server;PWD=sa;UID=sa;database=db;server=127.0.0.1
    std::string     m_strConnectionString;
    _ConnectionPtr  m_pDBConnection;
    
    DWORD           m_dwErrCode;
	EM_DATABASE_TYPE m_emDBType;
    std::string     m_strErrInfo;
    void SetLastError(DWORD dwErrCode) { m_dwErrCode = dwErrCode; }
    void SetLastError(_com_error& e)
    {
        //m_dwErrCode = e.WCode();
		m_dwErrCode = e.Error();
// 		_bstr_t strErr = e.Description();
//         m_strErrInfo = strErr;
    }
    void SetLastErrorInfo(const char* pszErrInfo) { m_strErrInfo = pszErrInfo; }
    inline void TESTHR(HRESULT x)  { if FAILED(x) _com_issue_error(x); }
};

