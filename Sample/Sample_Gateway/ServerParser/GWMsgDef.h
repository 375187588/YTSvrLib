#pragma once
#pragma pack(push,1)

//������Ϣ��ͷ��Ƕ���
#define TCPFLAG_GWMSG		0xFFEEDDCC
//������Ϣ���Ͷ���
#define T_GWMSG_KEEPALIVE			0	//����������
#define T_GWMSG_SVRLOGIN			1	//���ص�½
#define T_GWMSG_C2S					2	//�ͻ�����Ϣ
#define T_GWMSG_S2C					3	//������->�ͻ���
#define T_GWMSG_S2S					4	//������->������
#define T_GWMSG_S2S_ERR				5	//������������Ϣ
#define T_GWMSG_AS2US				6	
#define T_GWMSG_ADMIN2USER			7
#define T_GWMSG_CLIENTLOGOUT		8
#define T_GWMSG_CLIENTBROADCAST		10	//�ͻ��˹㲥��Ϣ

//�ͻ���/���������Ͷ���
typedef enum {
	emAgent_Null = 0,
	emAgent_Client = 1,	//�ͻ���
	emAgent_GateWay = 10,	//����
	emAgent_UserSvr = 20,	//�߼�������1
	emAgent_UserSvr2 = 30,	//�߼�������2
} EM_AGENT;

struct sAgent
{
	EM_AGENT m_emType;
	UINT m_nAgentID;
};

struct sGWMsg_Head
{
	sGWMsg_Head( UINT nMsgType ) : m_nTcpFlag(TCPFLAG_GWMSG),m_nMsgType(nMsgType)
	{
		m_nTotalMsgLen = sizeof(*this);
		ZeroMemory( &m_From, sizeof(m_From) );
		ZeroMemory( &m_To, sizeof(m_To) );
		ZeroMemory( &m_RouteBy, sizeof(m_RouteBy) );
	};
	void Init( UINT nMsgType, UINT nTotalLen ) {
		m_nTcpFlag = TCPFLAG_GWMSG;
		m_nTotalMsgLen = nTotalLen;
		m_nMsgType = nMsgType;
		ZeroMemory( &m_From, sizeof(m_From) );
		ZeroMemory( &m_To, sizeof(m_To) );
		ZeroMemory( &m_RouteBy, sizeof(m_RouteBy) );
	}
	UINT	m_nTcpFlag;			//�̶���ʶ����4Byte����
	UINT	m_nTotalMsgLen;  //�ܳ��ȣ�������ͷ����
	UINT	m_nMsgType;		
	
	char	m_szClientIP[32];//�ͻ���IP��ַ
	sAgent m_From;	//��������Ϣ
	sAgent m_To;	//���շ���Ϣ
	sAgent m_RouteBy;	//��ת����Ϣ
};

struct sGWMsg_KeepAlive : public sGWMsg_Head
{
	sGWMsg_KeepAlive() : sGWMsg_Head( T_GWMSG_KEEPALIVE )
	{
		m_nTotalMsgLen = sizeof(*this);
		m_nRunParam = 0;
	}
	UINT m_nRunParam;
};

struct sGWMsg_Login : public sGWMsg_Head
{
	sGWMsg_Login() : sGWMsg_Head( T_GWMSG_SVRLOGIN )
	{
		m_nTotalMsgLen = sizeof(*this);
		m_nGameID = 0;
	}
	UINT	m_nGameID;		//��ϷID
	UINT	m_nPublicSvrID;	//��������ID
};

struct sGWMsg_ClientDisconnect : public sGWMsg_Head
{
	sGWMsg_ClientDisconnect() : sGWMsg_Head( T_GWMSG_CLIENTLOGOUT )
	{
		m_nTotalMsgLen = sizeof(*this);
	}

	UINT m_nSvrID;
	UINT m_nGWID;
	UINT m_nClientID;
};

#pragma pack(pop)
