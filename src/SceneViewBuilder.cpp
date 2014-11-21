#include "SceneViewBuilder.h"

#include "CommandHandler.h"
#include "osgGA/TrackballManipulator"

using namespace VRSim3D;

SceneViewBuilder::SceneViewBuilder(void)
{
	m_SceneView = NULL;
}

SceneViewBuilder::~SceneViewBuilder(void)
{
}

bool VRSim3D::SceneViewBuilder::clearData()
{
	if(!m_SceneView.get())
		return false;
	DelEventHandlers();
	osg::ref_ptr<osg::Group> root = m_SceneView->m_Root.get();
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(getViewer());
	if(!viewer.valid())
		return false;
	osg::ref_ptr<VRSim3D::CommandHandler> cm = VRSim3D::CommandHandler::instance(viewer.get());
	for (size_t index = 0;index < root->getNumChildren();++index)
	{
		cm->addNodeCommand(root.get(),root->getChild(index));
	}

	return true;
} 

bool VRSim3D::SceneViewBuilder::createSceneView( HWND hwnd,const float &width /*= 800.0f*/,const float &height /*= 600.0f*/,const unsigned int &samples /*= 16*/ )
{
	m_SceneView->CreateScene(hwnd,width,height,samples);
	/*///zqgao 给主相机设置默认操控器
	AddCameraMP("Trackball",new osgGA::TrackballManipulator());*/
	m_KeySwitchMatrixManipulator = m_SceneView->getSwitchMatrixManipulator();
	m_MpNum = m_SceneView->m_MpNum;
	///zqgao 保存基础事件
	m_SceneView->SaveBasicEvents();

	m_BasicEvents = m_SceneView->m_BasicEvents;

	return true;
}

osg::Node* VRSim3D::SceneViewBuilder::getSceneData()
{
	osg::ref_ptr<osg::Group> root = m_SceneView->m_Root.get();

	return root.release();
}

osg::Vec4 VRSim3D::SceneViewBuilder::getViewRect()
{
	osg::Vec4 rect = osg::Vec4(0,0,0,0);
#ifdef _QTWIND
	rect.x() = m_SceneView->m_gWindow->getTraits()->x;
	rect.y() = m_SceneView->m_gWindow->getTraits()->y;
	rect.z() = m_SceneView->m_gWindow->getTraits()->width;
	rect.w() = m_SceneView->m_gWindow->getTraits()->height;
#else
	rect.x() = m_SceneView->m_gc->getTraits()->x;
	rect.y() = m_SceneView->m_gc->getTraits()->y;
	rect.z() = m_SceneView->m_gc->getTraits()->width;
	rect.w() = m_SceneView->m_gc->getTraits()->height;
#endif
	return rect;
}

