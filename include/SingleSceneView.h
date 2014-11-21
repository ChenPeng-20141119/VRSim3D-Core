//////////////////////////////////////////////////////////////////////////
///�ļ���������ͼ����
///�����ߣ���־��
///����ʱ�䣺2013��7��26��
///��������������ͼ����
//////////////////////////////////////////////////////////////////////////

#ifndef SINGLE_SCENE_VIEW_H
#define SINGLE_SCENE_VIEW_H 1

#include "Export.h"
#include "SceneView.h"

#include "osgViewer/Viewer"

namespace VRSim3D
{

class VRSim3D_EXPORT SingleSceneView :
	public SceneView
{
public:
	//friend class SceneViewBuilder;
	SingleSceneView(void);
	~SingleSceneView(void);
	///zqgao ��������������true�����ɹ�
	///\param samples(unsigned int)
	// ����ݱ����������ʹ�û��Կ�����
	virtual	bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);

	///zqgao ��ȡ�����Ӿ���
	virtual osgViewer::ViewerBase* getViewer();
private:
	// zqgao ��Ⱦ�߳�
	static void RenderThread(void* ptr);
	// zqgao ��ʼ�� 
	void myinit(void);
	///zqgao ��������¼�
	virtual void SaveBasicEvents();

protected:
	// zqgao ��ʼ�������
	void initCamera(void);
	// zqgao ���ó���
	void setupView(void);
	///zqgao ����������
	osg::ref_ptr<osgViewer::Viewer>						   m_Viewer;
};

}
#endif
