// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� VRSM3D_APP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// VRSM3D_APP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef VRSM3D_APP_EXPORTS
#define VRSM3D_APP_API __declspec(dllexport)
#else
#define VRSM3D_APP_API __declspec(dllimport)
#endif

// �����Ǵ� VRSM3D_APP.dll ������
class VRSM3D_APP_API CVRSM3D_APP {
public:
	CVRSM3D_APP(void);
	// TODO: �ڴ�������ķ�����
};

extern VRSM3D_APP_API int nVRSM3D_APP;

VRSM3D_APP_API int fnVRSM3D_APP(void);
