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
	// ���ô��ڵ�ͼ�����ԣ��������ڵ���ʾ��С���Լ�����ʹ�õ�Windows �����
	// ���������Դ��ݸ��½��Ĵ���ͼ��������
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
	///zqgao ����� ��Щ�Կ������ò���
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

// zqgao ��ʼ��
void VRSim3D::SceneView::init(void)
{

	m_Root = new osg::Group();
	m_Root->setName("Root");
	//m_Root->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//m_Root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);
	/// ������ʱ����Ϊ��̬���Ա����Ӽ��ٽڵ�ʱ������
	m_Root->setDataVariance(osg::Node::DataVariance::DYNAMIC);
	
	
#ifdef _QTWIND
	m_gWindow = NULL;
#else
	m_gc = NULL;
#endif
	//m_ControlCanvas = NULL;
}

#if 0
// zqgao ��Ⱦ�߳�
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

