#pragma once

#define AUTH_TICKET_OK		0
#define AUTH_TICKET_EXPIRED	1
#define AUTH_TICKET_INVALID	2

/*
����ֵ : 
0 = �ɹ�
1 = Ʊ���ѹ���
2 = Ʊ����Ч
*/
UINT CheckAuthTicket( __time32_t tNow,UINT nUserID,UINT nLastLogin, __time32_t tExpired, LPCWSTR pwzKey, LPCWSTR pwzTicket );
int	MakeAuthTicket( UINT nUserID, UINT nLastLogin, __time32_t tExpired, LPCWSTR pwzKey, wchar_t* pwzBuf, int nBufSize );

/*
����ֵ :
0 = �ɹ�
1 = Ʊ���ѹ���
2 = Ʊ����Ч
*/
UINT CheckLoginTicket( UINT nUserID, UINT nGameID,  LPCWSTR pwzPartnerID, LPCWSTR pwzPassport, LPCWSTR pwzKey, LPCWSTR pwzTicket );
int	MakeLoginTicket( UINT nUserID, UINT nGameID,  LPCWSTR pwzPartnerID, LPCWSTR pwzPassport, LPCWSTR pwzKey, wchar_t* pwzBuf, int nBufSize );