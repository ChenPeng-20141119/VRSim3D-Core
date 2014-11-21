#ifndef SINGLE_SCENE_VIEW_BUILLDER_H
#define SINGLE_SCENE_VIEW_BUILLDER_H 1

#include "Export.h"
#include "SceneViewBuilder.h"

#include "osgGA/KeySwitchMatrixManipulator"

namespace VRSim3D
{

class VRSim3D_EXPORT SingleSceneViewBuilder :
	public VRSim3D::SceneViewBuilder
{
public:
	SingleSceneViewBuilder(void);
	~SingleSceneViewBuilder(void);
	///zqgao ɾ������
	virtual bool delSceneView(); 
	///zqgao ���ó���������ٿ���
	virtual void setCameraMP(int key, std::string name, osgGA::CameraManipulator* cmp);
	///zqgao ���ӳ���������ٿ���
	virtual void AddCameraMP(std::string name, osgGA::CameraManipulator* cmp,bool current = true);
	///zqgao ɾ��������ٿ���
	virtual void DelCameraMP(std::string name);
	///zqgao ����¼�
	virtual bool AddEventHandler(osgGA::GUIEventHandler *handler,bool isBasick = false);
	///zqgao ɾ���¼�
	virtual bool RemoveEventHandler(osgGA::GUIEventHandler *handler);
	///zqgao ɾ���¼�
	virtual bool RemoveEventHandler(const std::string &handlerName);
	///zqgao ��������ü�
	virtual void setCameraCullBack(osg::NodeCallback *callback);
	///zqgao ��ȡ����ͼ�Ӿ���
	virtual osgViewer::ViewerBase* getViewer();
	///zqgao ��ȡ��������λ����Ϣ
	//virtual osg::Vec4 getViewRect();

	///zqgao  ���ӻ����¼�
	void AddBasicEvent(osgGA::GUIEventHandler *handler);
private:
	///zqgao ɾ�������ǻ����¼�
	virtual bool DelEventHandlers();

#if 0
private:
	///zqgao ����ͼ�Ӿ���
	osg::ref_ptr<osgViewer::Viewer>		m_viewer;
#endif

};

}
#endif
