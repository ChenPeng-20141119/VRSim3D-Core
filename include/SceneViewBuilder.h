//////////////////////////////////////////////////////////////////////////
///文件名：场景创建
///创建者：高志清
///创建时间：2013年7月26日
///内容描述：场景创建
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
	///zqgao 创建场景
	bool createSceneView(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);
	///zqgao 删除场景
	virtual bool delSceneView() = 0; 
	///zqgao  增加基础事件
	virtual void AddBasicEvent(osgGA::GUIEventHandler *handler) = 0;
	///zqgao 设置场景主相机操控器
	virtual void setCameraMP(int key, std::string name, osgGA::CameraManipulator* cmp) = 0;
	///zqgao 增加场景主相机操控器
	virtual void AddCameraMP(std::string name, osgGA::CameraManipulator* cmp,bool current = true) = 0;
	///zqgao 删除场景主相机操控器
	virtual void DelCameraMP(std::string name) = 0;
	///zqgao 添加事件
	virtual bool AddEventHandler(osgGA::GUIEventHandler *handler,bool isBasick = false) = 0;
	///zqgao 删除事件
	virtual bool RemoveEventHandler(osgGA::GUIEventHandler *handler) = 0;
	///zqgao 删除事件
	virtual bool RemoveEventHandler(const std::string &handlerName) = 0;
	///zqgao 设置相机裁剪
	virtual void setCameraCullBack(osg::NodeCallback *callback) = 0;
	///zqgao 获取场景视景器
	virtual osgViewer::ViewerBase* getViewer() = 0;
	///zqgao 获取视图场景
	SceneView* getSceneView()const{return m_SceneView.get();}

	///zqgao 清除场景数据
	bool clearData();
	///zqgao 获取场景根节点
	osg::Node* getSceneData();
	///zqgao 获取场景窗口位置信息
	osg::Vec4 getViewRect();
private:
	///zqgao 删除场景非基础事件
	virtual bool DelEventHandlers() = 0;
protected:
	///zqgao Viewer基础事件集（在场景切换时不需要更改事件）
	typedef std::list<osg::ref_ptr<osgGA::GUIEventHandler>> BASICEVENTS ;
	BASICEVENTS											   m_BasicEvents;
//#ifdef _QTWIND
//	osg::ref_ptr<SingleSceneViewQT>					m_SceneView;
//#else
	osg::ref_ptr<SceneView>								   m_SceneView;
//#endif
	///zqgao 相机操控器数量
	int													   m_MpNum;
	///zqgao 相机操控器
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>		   m_KeySwitchMatrixManipulator;
};

}
#endif
