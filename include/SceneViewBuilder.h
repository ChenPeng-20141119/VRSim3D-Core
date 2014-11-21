//////////////////////////////////////////////////////////////////////////
///�ļ�������������
///�����ߣ���־��
///����ʱ�䣺2013��7��26��
///������������������
//////////////////////////////////////////////////////////////////////////

#ifndef SCENE_VIEW_BUILLDER_H
#define SCENE_VIEW_BUILLDER_H 1

#include "Export.h"

#include "SceneView.h"
//#include "SingleSceneView-QT.h"


#include <osg/Referenced>

namespace VRSim3D
{

class VRSim3D_EXPORT SceneViewBuilder /*:
	public osg::Referenced*/
{
public:
	SceneViewBuilder(void);
	~SceneViewBuilder(void);
	///zqgao ��������
	bool createSceneView(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);
	///zqgao ɾ������
	virtual bool delSceneView() = 0; 
	///zqgao  ���ӻ����¼�
	virtual void AddBasicEvent(osgGA::GUIEventHandler *handler) = 0;
	///zqgao ���ó���������ٿ���
	virtual void setCameraMP(int key, std::string name, osgGA::CameraManipulator* cmp) = 0;
	///zqgao ���ӳ���������ٿ���
	virtual void AddCameraMP(std::string name, osgGA::CameraManipulator* cmp,bool current = true) = 0;
	///zqgao ɾ������������ٿ���
	virtual void DelCameraMP(std::string name) = 0;
	///zqgao ����¼�
	virtual bool AddEventHandler(osgGA::GUIEventHandler *handler,bool isBasick = false) = 0;
	///zqgao ɾ���¼�
	virtual bool RemoveEventHandler(osgGA::GUIEventHandler *handler) = 0;
	///zqgao ɾ���¼�
	virtual bool RemoveEventHandler(const std::string &handlerName) = 0;
	///zqgao ��������ü�
	virtual void setCameraCullBack(osg::NodeCallback *callback) = 0;
	///zqgao ��ȡ�����Ӿ���
	virtual osgViewer::ViewerBase* getViewer() = 0;
	///zqgao ��ȡ��ͼ����
	SceneView* getSceneView()const{return m_SceneView.get();}

	///zqgao �����������
	bool clearData();
	///zqgao ��ȡ�������ڵ�
	osg::Node* getSceneData();
	///zqgao ��ȡ��������λ����Ϣ
	osg::Vec4 getViewRect();
private:
	///zqgao ɾ�������ǻ����¼�
	virtual bool DelEventHandlers() = 0;
protected:
	///zqgao Viewer�����¼������ڳ����л�ʱ����Ҫ�����¼���
	typedef std::list<osg::ref_ptr<osgGA::GUIEventHandler>> BASICEVENTS ;
	BASICEVENTS											   m_BasicEvents;
//#ifdef _QTWIND
//	osg::ref_ptr<SingleSceneViewQT>					m_SceneView;
//#else
	osg::ref_ptr<SceneView>								   m_SceneView;
//#endif
	///zqgao ����ٿ�������
	int													   m_MpNum;
	///zqgao ����ٿ���
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>		   m_KeySwitchMatrixManipulator;
};

}
#endif
