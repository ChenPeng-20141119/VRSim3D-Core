// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VRSM3D_APP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VRSM3D_APP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef VRSM3D_APP_EXPORTS
#define VRSM3D_APP_API __declspec(dllexport)
#else
#define VRSM3D_APP_API __declspec(dllimport)
#endif

// 此类是从 VRSM3D_APP.dll 导出的
class VRSM3D_APP_API CVRSM3D_APP {
public:
	CVRSM3D_APP(void);
	// TODO: 在此添加您的方法。
};

extern VRSM3D_APP_API int nVRSM3D_APP;

VRSM3D_APP_API int fnVRSM3D_APP(void);
