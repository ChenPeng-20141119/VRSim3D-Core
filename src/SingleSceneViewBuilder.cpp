#include "SingleSceneViewBuilder.h"
#include "SingleSceneView.h"
#include "SingleSceneView-QT.h"
#include "UtileMethod.h"

using namespace VRSim3D;

SingleSceneViewBuilder::SingleSceneViewBuilder(void)
{
#ifdef _QTWIND
	m_SceneView = new SingleSceneViewQT();
#else
	m_SceneView = new SingleSceneView();
#endif
	
}

SingleSceneViewBuilder::~SingleSceneViewBuilder(void)
{
}

bool VRSim3D::SingleSceneViewBuilder::delSceneView()
{
	return false;
}

bool VRSim3D::SingleSceneViewBuilder::DelEventHandlers()
{
	osg::ref_ptr<osgViewer::Viewer> view = dynamic_cast<osgViewer::Viewer*>(m_SceneView->getViewer());
	BASICEVENTS eventHandlers = view->getEventHandlers();
	BASICEVENTS::iterator itor = eventHandlers.begin();
	BASICEVENTS::iterator basicitor = m_BasicEvents.begin();
	bool isBasic = false;
	while(itor != eventHandlers.end())
	{
		while(basicitor != m_BasicEvents.end())
		{
			if(*itor == *basicitor)
			{
				isBasic = true;
				break;
			}
			basicitor++;
		}
		if(!isBasic)
		{
			view->removeEventHandler(itor->get());	
		}
		isBasic = false;
		basicitor = m_BasicEvents.begin();
		itor++;
	}
	return true;
}

void VRSim3D::SingleSceneViewBuilder::AddBasicEvent( osgGA::GUIEventHandler *handler )
{
	m_BasicEvents.push_back(handler);
}

void VRSim3D::SingleSceneViewBuilder::setCameraMP( int key, std::string name, osgGA::CameraManipulator* cmp )
{
	if(!m_KeySwitchMatrixManipulator.get())
		m_KeySwitchMatrixManipulator = new osgGA::KeySwitchMatrixManipulator();
#if 0

	///zqgao 暂时没想好怎么写，需要继承KeySwitchMatrixManipulator增加部分功能
	///暂时用AddCameraMP代替

#endif

}

void VRSim3D::SingleSceneViewBuilder::AddCameraMP( std::string name, osgGA::CameraManipulator* cmp,bool current )
{
	if(m_KeySwitchMatrixManipulator.get())
	{
		m_KeySwitchMatrixManipulator->addMatrixManipulator(m_MpNum++,name,cmp);
		int index = UtileMethod::getMPIndexFromSwichMP(m_KeySwitchMatrixManipulator.get(),name);
		if(current)
			m_KeySwitchMatrixManipulator->selectMatrixManipulator(index);
	}
}

bool VRSim3D::SingleSceneViewBuilder::AddEventHandler( osgGA::GUIEventHandler *handler,bool isBasick/* = false */)
{
	osg::ref_ptr<osgViewer::Viewer> viewer  = dynamic_cast<osgViewer::Viewer*>(m_SceneView->getViewer());
	if(!viewer.get())
		return false;
	viewer->addEventHandler(handler);
	if(isBasick)
		AddBasicEvent(handler);
	return true;
}

bool VRSim3D::SingleSceneViewBuilder::RemoveEventHandler( osgGA::GUIEventHandler *handler )
{
	if(!handler)
		return false;
	osg::ref_ptr<osgViewer::Viewer> viewer  = dynamic_cast<osgViewer::Viewer*>(m_SceneView->getViewer());
	if(!viewer.get())
		return false;
	BASICEVENTS eventHandlers = viewer->getEventHandlers();
	BASICEVENTS::iterator itor = eventHandlers.begin();
	bool isBasic = false;
	while(itor != eventHandlers.end())
	{
		if(*itor == handler)
		{
			viewer->removeEventHandler(itor->get());	
			return true;
		}
		itor++;
	}
	return false;
}

bool VRSim3D::SingleSceneViewBuilder::RemoveEventHandler( const std::string &handlerName )
{
	if(handlerName.empty())
		return false;
	osg::ref_ptr<osgViewer::Viewer> viewer  = dynamic_cast<osgViewer::Viewer*>(m_SceneView->getViewer());
	if(!viewer.get())
		return false;
	BASICEVENTS eventHandlers = viewer->getEventHandlers();
	BASICEVENTS::iterator itor = eventHandlers.begin();
	bool isBasic = false;
	while(itor != eventHandlers.end())
	{
		if(itor->get()->getName() == handlerName)
		{
			viewer->removeEventHandler(itor->get());	
			return true;
		}
		itor++;
	}
	return false;
}

void VRSim3D::SingleSceneViewBuilder::setCameraCullBack( osg::NodeCallback *callback )
{
	osg::ref_ptr<osgViewer::Viewer> viewer  = dynamic_cast<osgViewer::Viewer*>(m_SceneView->getViewer());
	osg::ref_ptr<osg::Camera> cm = viewer->getCamera();
	cm->setCullCallback(callback);
}

void VRSim3D::SingleSceneViewBuilder::DelCameraMP( std::string name )
{
	UtileMethod::RemoveMPIndexFromSwichMP(m_KeySwitchMatrixManipulator.get(),name);

	m_KeySwitchMatrixManipulator->selectMatrixManipulator(0);
}

osgViewer::ViewerBase* VRSim3D::SingleSceneViewBuilder::getViewer()
{
	osg::ref_ptr<osgViewer::Viewer> viewer  = dynamic_cast<osgViewer::Viewer*>(m_SceneView->getViewer());

	if(viewer.get())
		return viewer.release();
	return NULL;
}

#if 0
osg::Vec4 VRSim3D::SingleSceneViewBuilder::getViewRect()
{
	osg::Vec4 rect = osg::Vec4(0,0,0,0);

	rect.x() = m_SceneView->m_gc->getTraits()->x;
	rect.y() = m_SceneView->m_gc->getTraits()->y;
	rect.z() = m_SceneView->m_gc->getTraits()->width;
	rect.w() = m_SceneView->m_gc->getTraits()->height;

	return rect;
}

#endif
