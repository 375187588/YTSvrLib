#pragma once
#pragma pack(push,1)

#define TCPFLAG_SIGN_CLIENTMSG     0xFFFFEEEE

typedef int			USERID;
typedef int			NUMBER;
typedef char		STRING;
typedef int			TIME;

enum EM_MSG_TYPE
{

};

#define RESP(_Msg) (_Msg+0x1000)

struct sClientMsg_Head
{
	sClientMsg_Head(UINT nMsgType) : m_nTCPFlag(TCPFLAG_SIGN_CLIENTMSG), m_nMsgType(nMsgType), m_nMsgLenTotal(sizeof(sClientMsg_Head)),m_nMsgSeqNo(0) {
		m_nZipEncrypFlag = 0;
		m_nZipSrcLen = 0;
	};
	void InitHead( UINT nMsgSeqno,UINT nMsgType, UINT nTotalLen ){
		m_nTCPFlag = TCPFLAG_SIGN_CLIENTMSG;
		m_nMsgType = nMsgType;
		m_nMsgLenTotal = nTotalLen;
		m_nMsgSeqNo = nMsgSeqno;
		m_nZipEncrypFlag = 0;
		m_nZipSrcLen = 0;
	}
	UINT m_nTCPFlag;	//�̶���ǣ�1 UINT��0xFFFFEEEE
	UINT m_nMsgType;	//��Ϣ���ͣ�1 UINT��0x0001~0x0FFF
	UINT m_nMsgSeqNo;		//��Ϣ��ţ�1 UINT������
	UINT m_nMsgLenTotal;	//��Ϣ�ܳ��ȣ�1 UINT��������Ϣͷ����
#define _CLIENTMSG_FLAG_ENCRYPT			0x1
#define	_CLIENTMSG_FLAG_ZIPPED			0x2
	UINT m_nZipEncrypFlag;	//��Ϣ��ѹ��/���ܱ�ǣ�UINT�� 0x1=ѹ��|0x2=����
	UINT m_nZipSrcLen;	//ѹ��ǰ��Ϣ��ԭ���ȣ�UINT��
};
#define MAX_DATA_TO_ZIP		(8192<<4)

//�ͻ���������Ϣͷ�ṹ���壺
struct sClientMsg_ReqHead : public sClientMsg_Head
{
	sClientMsg_ReqHead( UINT nMsgType) : sClientMsg_Head(nMsgType)
	{
		m_nUserID = 0;
		m_nMsgLenTotal = sizeof(sClientMsg_ReqHead);
	}
	void InitHead(UINT nMsgSeqno, UINT nMsgType, UINT nTotalLen)
	{
		sClientMsg_Head::InitHead( nMsgSeqno,nMsgType, nTotalLen );
		m_nUserID = 0;
	}
	BOOL CheckData( int nLen ) {
		if( nLen < sizeof(sClientMsg_ReqHead) )
			return FALSE;
		return TRUE;
	}

	USERID m_nUserID;	//��ұ��
};

//��������Ӧ��Ϣͷ�ṹ���壺
struct sClientMsg_RespHead : public sClientMsg_Head
{
	sClientMsg_RespHead( UINT nMsgType) : sClientMsg_Head(nMsgType)
	{
		m_nRespRet = 0;
		m_nMsgLenTotal = sizeof(sClientMsg_RespHead);
	}
	void InitHead(UINT nMsgSeqno, UINT nMsgType, UINT nTotalLen)
	{
		sClientMsg_Head::InitHead( nMsgSeqno,nMsgType, nTotalLen );
		m_nRespRet = 0;
	}
	NUMBER m_nRespRet;	//��Ӧ�����1 UINT��
};

//������֪ͨ��Ϣͷ�ṹ����
struct sClientNotify_Head : public sClientMsg_Head
{
	sClientNotify_Head( UINT nMsgType) : sClientMsg_Head(nMsgType)
	{
		m_nMsgLenTotal = sizeof(sClientNotify_Head);
		m_nRet = 0;
	}

	NUMBER m_nRet;
};

#pragma pack(pop)
