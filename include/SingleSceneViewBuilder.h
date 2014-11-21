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
	///zqgao 删除场景
	virtual bool delSceneView(); 
	///zqgao 设置场景主相机操控器
	virtual void setCameraMP(int key, std::string name, osgGA::CameraManipulator* cmp);
	///zqgao 增加场景主相机操控器
	virtual void AddCameraMP(std::string name, osgGA::CameraManipulator* cmp,bool current = true);
	///zqgao 删除场相机操控器
	virtual void DelCameraMP(std::string name);
	///zqgao 添加事件
	virtual bool AddEventHandler(osgGA::GUIEventHandler *handler,bool isBasick = false);
	///zqgao 删除事件
	virtual bool RemoveEventHandler(osgGA::GUIEventHandler *handler);
	///zqgao 删除事件
	virtual bool RemoveEventHandler(const std::string &handlerName);
	///zqgao 设置相机裁剪
	virtual void setCameraCullBack(osg::NodeCallback *callback);
	///zqgao 获取单视图视景器
	virtual osgViewer::ViewerBase* getViewer();
	///zqgao 获取场景窗口位置信息
	//virtual osg::Vec4 getViewRect();

	///zqgao  增加基础事件
	void AddBasicEvent(osgGA::GUIEventHandler *handler);
private:
	///zqgao 删除场景非基础事件
	virtual bool DelEventHandlers();

#if 0
private:
	///zqgao 单视图视景器
	osg::ref_ptr<osgViewer::Viewer>		m_viewer;
#endif

};

}
#endif
