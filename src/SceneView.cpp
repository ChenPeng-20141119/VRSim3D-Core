#include "SceneView.h"

#include "UtileMethod.h"
#include "MouseCoordsHandler.h"

#include "osgDB/ReadFile"
#include "osgDB/WriteFile"
#include <osgDB/FileUtils>

#include "osgViewer/api/win32/GraphicsWindowWin32"
#include "osgViewer/ViewerEventHandlers"

#include "osgGA/KeySwitchMatrixManipulator"
#include "osgGA/StateSetManipulator"
#include "osgGA/GUIEventHandler"
#include "osgGA/EventQueue"


#include "osgEarth/NodeUtils"
#include "osgEarthUtil/AutoClipPlaneHandler"
#include "osgEarth/Viewpoint"
#include "osgEarthUtil/EarthManipulator"

#include <process.h>
#include <WinBase.h>

using namespace VRSim3D;

SceneView::SceneView(void)
{
	init();
}
VRSim3D::SceneView::~SceneView( void )
{
}
VRSim3D::SceneView::SceneView( HWND hwnd,const float &width,const float &height )
{
	if(!hwnd)
	{
	}
		//SceneView(width,height);

}

void VRSim3D::SceneView::setGraphicsContext( HWND hwnd, const float & width, const float & height,const unsigned int &samples )
{
	// 设置窗口的图形属性，包括窗口的显示大小，以及它所使用的Windows 句柄等
	// 将窗口属性传递给新建的窗口图形上下文
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 0;
	traits->y = 0;
	/*traits->width = rect.right - rect.left ;
	traits->height =GetSystemMetrics(SM_CXSCREEN)* 
		(rect.bottom - rect.top)/GetSystemMetrics(SM_CYSCREEN);*/
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	///zqgao 抗锯齿 有些显卡可能用不了
	traits->samples = samples;
#ifdef _QTWIND
	if(m_gWindow.valid())
		return;
	m_gWindow = new osgViewer::GraphicsWindowEmbedded(traits.get());
#else
	if (hwnd)
	{
		osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData( hwnd);
		traits->inheritedWindowData = windata;
	}
	m_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
#endif
	
}

// zqgao 初始化
void VRSim3D::SceneView::init(void)
{

	m_Root = new osg::Group();
	m_Root->setName("Root");
	//m_Root->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//m_Root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);
	/// 这里暂时设置为动态，以便增加减少节点时出问题
	m_Root->setDataVariance(osg::Node::DataVariance::DYNAMIC);
	
	
#ifdef _QTWIND
	m_gWindow = NULL;
#else
	m_gc = NULL;
#endif
	//m_ControlCanvas = NULL;
}

#if 0
// zqgao 渲染线程
void VRSim3D::SceneView::RenderThread(void* ptr)
{
	osg::ref_ptr<osgViewer::Viewer> view = (osgViewer::Viewer*)(ptr);
	if (view )
	{
		while(!view->done())
		{
			view->frame();
		}
		_endthread();
	}
}

#endif
osgGA::KeySwitchMatrixManipulator* VRSim3D::SceneView::getSwitchMatrixManipulator()
{
	return m_KeySwitchMatrixManipulator.get();
}

osg::GraphicsContext* VRSim3D::SceneView::getGraphicsContext() const
{
#ifdef _QTWIND
	return m_gWindow.get();
#else
	return m_gc.get();
#endif
}

