#pragma once
#pragma pack(push,1)

//������Ϣ��ͷ��Ƕ���
#define TCPFLAG_GWMSG		0x00000001

//�ͻ���/���������Ͷ���
typedef enum
{
	emAgent_Null = 0,
	emAgent_Client = 1,	//�ͻ���
	emAgent_GateWay = 10,	//����
} EM_AGENT;

struct sAgent
{
	EM_AGENT m_emType;
	UINT m_nAgentID;
};

struct sGWMsg_Head
{
	sGWMsg_Head(UINT nMsgType) : m_nTcpFlag(TCPFLAG_GWMSG), m_nMsgType(nMsgType)
	{
		m_nTotalMsgLen = sizeof(*this);
		ZeroMemory(&m_From, sizeof(m_From));
		ZeroMemory(&m_To, sizeof(m_To));
		ZeroMemory(&m_RouteBy, sizeof(m_RouteBy));
	};
	void Init(UINT nMsgType, UINT nTotalLen)
	{
		m_nTcpFlag = TCPFLAG_GWMSG;
		m_nTotalMsgLen = nTotalLen;
		m_nMsgType = nMsgType;
		ZeroMemory(&m_From, sizeof(m_From));
		ZeroMemory(&m_To, sizeof(m_To));
		ZeroMemory(&m_RouteBy, sizeof(m_RouteBy));
	}
	UINT	m_nTcpFlag;			//�̶���ʶ����4Byte����
	UINT	m_nTotalMsgLen;  //�ܳ��ȣ�������ͷ����
	UINT	m_nMsgType;

	char	m_szClientIP[32];//�ͻ���IP��ַ
	sAgent m_From;	//��������Ϣ
	sAgent m_To;	//���շ���Ϣ
	sAgent m_RouteBy;	//��ת����Ϣ
};

#pragma pack(pop)
