#include "GlobeSceneViewManager.h"
#include "MouseCoordsHandler.h"

#include "osgEarthUtil/AutoClipPlaneHandler"
#include "osgEarthUtil/ContourMap"

#include "osgDB/FileUtils"

using namespace VRSim3D;

GlobeSceneViewManager::GlobeSceneViewManager(void)
{
	m_MapNode = NULL;
	m_EarthManip = NULL;
	m_ControlCanvas = NULL;
}

GlobeSceneViewManager::~GlobeSceneViewManager(void)
{
	///zqgao 清除地球数据
	ClearGlobeData();
}

VRSim3D::GlobeSceneViewManager::GlobeSceneViewManager( SceneViewBuilder *sceneViewBuilder )
:SceneViewManager(sceneViewBuilder)
{
	m_MapNode = NULL;
	m_EarthManip = NULL;
}

bool VRSim3D::GlobeSceneViewManager::Load( const std::string &path )
{
	if(m_MapNode.get())
		return false;
	std::string fileName = osgDB::findDataFile(path, 0);
	osg::ref_ptr<osg::Node> globeNode;
	if(!fileName.empty())
		globeNode = osgDB::readNodeFile(fileName);
	m_MapNode = osgEarth::MapNode::findMapNode(globeNode.get()); 
	globeNode->setNodeMask(NM_YES_REC_INTER);
	if (m_MapNode.valid())//("获取影像根节点失败！");
	{
		///zqgao 添加地形等高轮廓
		osg::ref_ptr<osgEarth::Util::ContourMap> cmap = new osgEarth::Util::ContourMap();
		///zqgao 设置不透明度
		cmap->setOpacity(0.2f);
		m_MapNode->getTerrainEngine()->addEffect(cmap.get());
		m_MapNode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);
		AddNode(m_ModelRoot.get(),m_MapNode.get());
		///zqgao 添加地球操控器
		setEarthManip();
		///zqgao 相机裁剪
		setCameraCull();	
		///zqgao 显示鼠标位置（与球面交点）
		showMousePosition();
		///zqgao 显示屏幕中心位置
		showViewPosition();
	}
	return true;
}
  
void VRSim3D::GlobeSceneViewManager::setEarthManip()
{
	if(m_EarthManip.get())
		return;
	m_EarthManip = new osgEarth::Util::EarthManipulator();
	//m_EarthManip->setIntersectTraversalMask(0xF0000001);
	m_EarthManip->getSettings()->setTetherMode(osgEarth::Util::EarthManipulator::TETHER_CENTER_AND_HEADING);
	m_EarthManip->getSettings()->setArcViewpointTransitions(true);
	double n = m_EarthManip->getSettings()->getMinDistance();
	m_EarthManip->getSettings()->setMinMaxDistance(n, 348062470);

	m_EarthManip->setHomeViewpoint( osgEarth::Viewpoint(osg::Vec3d(118,25,0), 0, -89,
		m_MapNode->getMap()->getProfile()->getSRS()->getEllipsoid()->getRadiusEquator()*3.0 ), 2); 

	m_EarthManip->setViewpoint(osgEarth::Viewpoint(osg::Vec3d(119,24,0), 0, -89,
		m_MapNode->getMap()->getProfile()->getSRS()->getEllipsoid()->getRadiusEquator()*3.0 ),2.0);
	m_EarthManip->setName("EarthManip");
	m_SceneViewBuilder->AddCameraMP("EarthManip",m_EarthManip.get());

}

void VRSim3D::GlobeSceneViewManager::setCameraCull()
{
	///针对球面场景，自动调整远近裁剪面，避免接近地面时切掉近处场景地面
	osg::ref_ptr<osgEarth::Util::AutoClipPlaneCullCallback> pAt = 
	new osgEarth::Util::AutoClipPlaneCullCallback(m_MapNode.get());
	pAt->setClampFarClipPlane(false);
	m_SceneViewBuilder->setCameraCullBack(pAt.get());
}

bool VRSim3D::GlobeSceneViewManager::ClearGlobeData()
{
	///zqgao 删除鼠标信息
	DelMouseInfor();
	//zqgao 删除屏幕中心点信息
	DelViewPosInfoText();
	RemoveNode(m_ControlCanvas->getParent(0),m_ControlCanvas.get());
	//m_ControlCanvas->getParent(0)->removeChild(m_ControlCanvas.get());
	m_ControlCanvas.release();
	m_ControlCanvas = NULL;
	///zqgao 删除地球
	if(m_MapNode->getParent(0))
		//m_MapNode->getParent(0)->removeChild(m_MapNode.get());
		RemoveNode(m_MapNode->getParent(0),m_MapNode.get());
	m_MapNode.release();
	m_MapNode = NULL;
	///zqgao 接触地球操控器
	if(m_EarthManip.get())
	{
		m_SceneViewBuilder->DelCameraMP("EarthManip");
		m_EarthManip.release();
		m_EarthManip = NULL;
	}
	return true;
}

void VRSim3D::GlobeSceneViewManager::showMouseInfor( bool isShow )
{
	if(m_MouseCoords.get())
	{
		if(m_MouseCoords->visible() != isShow )
			m_MouseCoords->setVisible(isShow);
	}
}

void VRSim3D::GlobeSceneViewManager::showViewPosInfoText( bool isShow /*= true*/ )
{
	if(!m_ViewPosInfoText.get())
		return;
	if(m_ViewPosInfoText->getVisible() != isShow)
		m_ViewPosInfoText->setVisible(isShow);
}

bool VRSim3D::GlobeSceneViewManager::DelMouseInfor()
{
	if (!m_MouseCoords.get())
		return false;
	///zqgao 删除鼠标位置信息节点
	m_ControlCanvas->removeControl(m_MouseCoords.get());

	///zqgao 释放鼠标删除鼠标位置信息节点
	m_MouseCoords.release();
	m_MouseCoords = NULL;

	///zqgao 删除鼠标位置实时信息
	m_SceneViewBuilder->RemoveEventHandler("MouseCoordsHandler");
	return true;
}

bool VRSim3D::GlobeSceneViewManager::DelViewPosInfoText()
{
	if (!m_ViewPosInfoText.get())
		return false;
	osg::ref_ptr<osg::Node> inforNode = m_ViewPosInfoText->getInforNode();
	if(inforNode.get())
	{
		inforNode->getParent(0)->removeChild(inforNode.get());
		//RemoveNode(inforNode->getParent(0),inforNode.get());
		///zqgao 释放
		m_ViewPosInfoText.release();
		m_ViewPosInfoText = NULL;
	}

	return true;
}

void VRSim3D::GlobeSceneViewManager::showMousePosition( bool isShow /*= true*/ )
{
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(m_SceneViewBuilder->getViewer());
	if(!viewer.get())
		return;
	///zqgao 控件面板层 控件显示相当于HUDCamera，需要场景camera创建后才有意义
	if(!m_ControlCanvas.get())
	{
		m_ControlCanvas = osgEarth::Util::Controls::ControlCanvas::get(viewer.get());
		m_ControlCanvas->setNodeMask(NM_GUI_MANAGER);
		//m_ControlCanvas->setAllowEventFocus(true);
		m_ControlCanvas->setName("ControlCanvas");
	}
	if(m_MouseCoords.get())
		return;
	///dbs 首先检查变量参数合法性
	if (!m_MapNode.valid())  return;

	///dbs 提取字体文件，并判断其合法性
	if (!m_AllFont.valid())   return;

	///dbs 创建用于显示鼠标实时位置的控件，并将其添加到面板中
	m_MouseCoords = new osgEarth::Util::Controls::LabelControl();
	m_MouseCoords->setFont(m_AllFont.get());
	m_MouseCoords->setFontSize(12);
	//mouseCoords->setForeColor()
	m_MouseCoords->setEncoding(osgText::String::ENCODING_UTF8);
	m_MouseCoords->setHorizAlign(Control::ALIGN_CENTER);
	m_MouseCoords->setVertAlign(Control::ALIGN_TOP);
	m_MouseCoords->setBackColor(0.0, 0.2, 0.2, 0.4);    
	m_MouseCoords->setSize(400, 50);
	m_MouseCoords->setMargin(8);

	m_ControlCanvas->addControl(m_MouseCoords.get());

	showMouseInfor(isShow);

	///dbs 鼠标位置实时显示
	osg::ref_ptr<MouseCoordsHandler> mouseCoordsHandler = new MouseCoordsHandler(m_MouseCoords.get(), m_MapNode.get());
	mouseCoordsHandler->setName("MouseCoordsHandler");
	m_SceneViewBuilder->AddEventHandler(mouseCoordsHandler.get());
	//m_Viewer->addEventHandler(mMouseCoordsHandler.get());
}

void VRSim3D::GlobeSceneViewManager::showViewPosition( bool isShow /*= true*/ )
{
	m_ViewPosInfoText = new ViewPosInfoText(m_EarthManip.get());
	
	osg::Vec4 rect = m_SceneViewBuilder->getViewRect();
	osg::ref_ptr<osg::Node> node = m_ViewPosInfoText->makeViewPosInfoText(0,0,
		rect.z(),rect.w());
	//AddNode(m_ControlCanvas.get(),node.get());
	//m_ControlCanvas->addChild(node.get());
	AddNode(m_ModelRoot.get(),node.get());

	showViewPosInfoText(isShow);
}

void VRSim3D::GlobeSceneViewManager::flyTo( const float& lat,const float &lon,const float &zoom /*= 5000*/, const float &heading_deg /*= 73.04*/,const float &pitch_deg /*= -20.0*/,const float &time /*= 5.0*/ )
{
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(m_SceneViewBuilder->getViewer());
	if(!viewer.get())
		return;
	osgGA::KeySwitchMatrixManipulator*   swthmanp = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(viewer->getCameraManipulator());
	osgEarth::Util::EarthManipulator* earthManp = dynamic_cast<osgEarth::Util::EarthManipulator*>(swthmanp->getCurrentMatrixManipulator());
	if(earthManp)
	{
		earthManp->setViewpoint(osgEarth::Viewpoint(osg::Vec3f(lon,lat,0),heading_deg,pitch_deg,zoom,m_MapNode->getMapSRS()),time);
	}
}

void VRSim3D::GlobeSceneViewManager::cameraBound( osg::Node* node,const float &distance )
{
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(m_SceneViewBuilder->getViewer());
	if(!viewer.get())
		return;
	osgGA::KeySwitchMatrixManipulator*   swthmanp = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(viewer->getCameraManipulator());
	osgEarth::Util::EarthManipulator* earthManp = dynamic_cast<osgEarth::Util::EarthManipulator*>(swthmanp->getCurrentMatrixManipulator());
	if(earthManp)
	{
		earthManp->setTetherNode(node);
		earthManp->setDistance(distance);
	}
}

void VRSim3D::GlobeSceneViewManager::releaseCameraBound()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(m_SceneViewBuilder->getViewer());
	if(!viewer.get())
		return;
	osgGA::KeySwitchMatrixManipulator*   swthmanp = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(viewer->getCameraManipulator());
	osgEarth::Util::EarthManipulator* earthManp = dynamic_cast<osgEarth::Util::EarthManipulator*>(swthmanp->getCurrentMatrixManipulator());
	if(earthManp)
	{
		earthManp->setTetherNode(NULL);
	}
}
