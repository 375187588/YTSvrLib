#pragma once





#ifdef DEBUG
    #define ODS(s)      OutputDebugString(s)
#else
    #define ODS(s)
#endif


//����ʱ��ʾ�����е�TODO�б��ATTENTION�б�
//#pragma TODO("your description")
//#pragma ATTENTION("your description")
#define TOSTR(x)        #x
#define TOSTR2(x)       TOSTR(x)
#define TODO(x)         message(__FILE__ "(" TOSTR2(__LINE__) "): TODO " x)
#define ATTENTION(x)    message(__FILE__ "(" TOSTR2(__LINE__) "): ATTENTION " x)

