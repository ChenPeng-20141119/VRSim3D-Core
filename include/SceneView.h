//////////////////////////////////////////////////////////////////////////
///文件名：视图场景
///创建者：高志清
///创建时间：2013年7月26日
///内容描述：视图场景
//////////////////////////////////////////////////////////////////////////
#ifndef SCENEVIEW_H
#define SCENEVIEW_H 1

#include "Export.h"

#include "osg/GraphicsContext"
#include "osg/Group"
#include "osgGA/KeySwitchMatrixManipulator"

#include "osgViewer/ViewerBase"

#include "osgEarthUtil/Controls"

#include <Windows.h>

#ifdef _QTWIND
#include <QtGui/QGraphicsView>
//#include <graphicsview/qgraphicssceneevent>
#include <qgraphicssceneevent.h>
#include <QString>
#include <QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QMdiArea>

#endif

namespace VRSim3D
{
class VRSim3D_EXPORT SceneView : public osg::Referenced
{
public:
	friend class SceneViewBuilder;
	SceneView(void);
	~SceneView(void);
	
	///zqgao 用于嵌入Cwnd窗口
	///\param hwnd(HWND)
	// 嵌入窗口句柄，若为空则创建独立窗口
	SceneView(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f);
	
	///zqgao 创建场景，返回true创建成功
	///\param samples(unsigned int)
	// 抗锯齿倍数，请根据使用机显卡设置
	virtual bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16) = 0;
	
	///zqgao 保存基础事件
	virtual void SaveBasicEvents() = 0;
	
	///zqgao 获取场景视景器
	virtual osgViewer::ViewerBase* getViewer() = 0;

	///zqgao 获取上下文环境
	osg::GraphicsContext* getGraphicsContext() const;

	///zqgao 获取相机操控器
	osgGA::KeySwitchMatrixManipulator* getSwitchMatrixManipulator();

protected:
	// zqgao 创建上下文环境
	void setGraphicsContext(HWND hwnd, const float & width, const float & height,const unsigned int &samples);
	// zqgao 初始化 
	void init(void);
#if 0
private:
	// zqgao 渲染线程
	static void RenderThread(void* ptr);
#endif
protected:	
	///zqgao 根节点
	osg::ref_ptr<osg::Group>							   m_Root;
	///zqgao 相机操控器
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator>		   m_KeySwitchMatrixManipulator;
	///zqgao 相机操控器数量
	int													   m_MpNum;
	///zqgao Viewer基础事件集（在场景切换时不需要更改事件）
	typedef std::list<osg::ref_ptr<osgGA::GUIEventHandler>> BASICEVENTS ;
	BASICEVENTS											   m_BasicEvents;

///zqgao 上下文环境
#ifdef _QTWIND
	osg::ref_ptr<osgViewer::GraphicsWindow>	m_gWindow;
#else
	osg::ref_ptr<osg::GraphicsContext>					   m_gc;
#endif
		
};
}
#endif
