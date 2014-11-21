// VRSM3D_APP.cpp : 定义 DLL 应用程序的入口点。
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

// 这是导出变量的一个示例
VRSM3D_APP_API int nVRSM3D_APP=0;

// 这是导出函数的一个示例。
VRSM3D_APP_API int fnVRSM3D_APP(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 VRSM3D_APP.h
CVRSM3D_APP::CVRSM3D_APP()
{
	return;
}
