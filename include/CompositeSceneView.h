//////////////////////////////////////////////////////////////////////////
///�ļ���������ͼ����
///�����ߣ���־��
///����ʱ�䣺2013��7��26��
///��������������ͼ����
//////////////////////////////////////////////////////////////////////////

#ifndef COMPOSITE_SCENE_VIEW_H
#define COMPOSITE_SCENE_VIEW_H 1

#include "Export.h"
#include "sceneview.h"

#include "osgViewer/CompositeViewer"

namespace VRSim3D
{

class VRSim3D CompositeSceneView :
	public VRSim3D::SceneView
{
public:
	enum Style
	{
		ST_AUTOMATIC,
		ST_VERTICAL,
		ST_HORIZONTAL

	};
	CompositeSceneView(void);
	~CompositeSceneView(void);
	///zqgao ��������������true�����ɹ�
	///\param samples(unsigned int)
	// ����ݱ����������ʹ�û��Կ�����
	virtual	bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);
private:
	// zqgao ��Ⱦ�߳�
	static void RenderThread(void* ptr);
	// zqgao ��ʼ�� 
	void myinit(void);
	// zqgao ���ó���
	void setupView(const int &index);
	// zqgao ��ʼ�������
	void initCamera(void);
	///zqgao ��������ٿ���
	void setCmaraMP(osgViewer::View* view,osgGA::CameraManipulator* cm,std::string name);
	///zqgao �ӿڲ���
	void setCompostViewers(Style style = ST_AUTOMATIC);
	///zqgao �ָ�
	void partition(Style style);
	///zqgao ����������ԣ�viewPort�ȣ�
	void setCameraPPT(int numview,int allview,Style style);
	//zqgao �Զ���������
	void setViewPos(const int& numview,const int& allview,float& x, float& y, float& width, float& hight);
private:
	///zqgao ����������
	osg::ref_ptr<osgViewer::CompositeViewer>		m_Viewer;
	///zqgao ����ٿ���
	std::vector<osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>> m_KeySwitchMatrixManipulatorMap;
	///zqgao ����ٿ�������
	int													   m_MpNum;
};
}
#endif
