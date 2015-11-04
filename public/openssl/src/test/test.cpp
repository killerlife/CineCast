// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "test.h"

#include "rsa_SpriteLW.h"

#include <sys/stat.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
	}
    
    FILE* fp = fopen(".key.u.pem", "rb");
    if(!fp) return -1;
    
    struct  stat st;
    stat(".key.u.pem", &st);
    char * keyBuff = new char[st.st_size+1];
    fread(keyBuff, 1, st.st_size, fp);
    keyBuff[st.st_size] = 0;
    fclose(fp);
    
    g_pEvpPrvKey_r = ExtractPrivateKey(CString(keyBuff));
    delete keyBuff;
    
    g_pEvpPubKey_i   =   ExtractPublicKey(CString(PUBKEY));
    g_pEvpPrvKey_r   =   ExtractPrivateKey(CString(PRVKEY_r));
    
    //    CWinThread   *   pThread1;
    int   i=   100;
//    while(i--)
    if(g_pEvpPubKey_i && g_pEvpPrvKey_r)
    {
        //        pThread1   =   AfxBeginThread(THREAPROC,0);
        THREAPROC(0);
    }
    
    //    WaitForSingleObject(pThread1-> m_hThread,(DWORD)-1);
    getchar();
    
	return nRetCode;
}


