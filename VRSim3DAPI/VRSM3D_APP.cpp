// VRSM3D_APP.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "VRSM3D_APP.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// ���ǵ���������һ��ʾ��
VRSM3D_APP_API int nVRSM3D_APP=0;

// ���ǵ���������һ��ʾ����
VRSM3D_APP_API int fnVRSM3D_APP(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� VRSM3D_APP.h
CVRSM3D_APP::CVRSM3D_APP()
{
	return;
}
