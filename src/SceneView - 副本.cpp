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
#include "osgGA/TrackballManipulator"

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
	if(m_Viewer.get())
	{
		m_Viewer->stopThreading();
		m_Viewer->setDone(true);
		Sleep(100);
		delete m_Viewer;
	}
}
VRSim3D::SceneView::SceneView( HWND hwnd,const float &width,const float &height )
{
	if(!hwnd)
	{
	}
		//SceneView(width,height);

}

bool VRSim3D::SceneView::CreateScene( HWND hwnd,const float &width /*= 800.0f*/,const float &height /*= 600.0f*/,const unsigned int &samples /*= 16*/ )
{
	///zqgao 上下文环境
	setGraphicsContext(hwnd,width,height,samples);
	///zqgao 设置场景
	setupView();
	return false;
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
	if (hwnd)
	{
		osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData( hwnd);
		traits->inheritedWindowData = windata;
	}
	m_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
}

// zqgao 初始化
void VRSim3D::SceneView::init(void)
{

	m_Viewer = new osgViewer::Viewer();
	m_KeySwitchMatrixManipulator = new osgGA::KeySwitchMatrixManipulator();
	///zqgao 相当于char '0',便于用键盘切换操控器
	m_MpNum = 48;
	m_KeySwitchMatrixManipulator->addMatrixManipulator(m_MpNum++,"Trackball",new osgGA::TrackballManipulator());
	m_KeySwitchMatrixManipulator->selectMatrixManipulator(0);
	m_Viewer->setCameraManipulator(m_KeySwitchMatrixManipulator.get());
	m_KeySwitchMatrixManipulator->setName("SwichMP");

	m_Root = new osg::Group();
	m_Root->setName("Root");
	/// 这里暂时设置为动态，以便增加减少节点时出问题
	m_Root->setDataVariance(osg::Node::DataVariance::DYNAMIC);
	
	///zqgao 设备层
	m_ModelRoot = new osg::Group();
	m_ModelRoot->setName("ModelRoot");
	m_Root->addChild(m_ModelRoot.get());

	m_Dir = GetAppPath()  + "3Ddata" + "/";
	///zqgao 设置默认数据路径，用于相对目录
	osgDB::getDataFilePathList().push_back(m_Dir);
	m_AllFont = osgText::readFontFile(m_Dir+"fonts/simhei.ttf");

	m_CommandHandler = new VRSim3D::CommandHandler();
	m_Root->addEventHandler(m_CommandHandler.get());
	m_Root->addEventCallback()
	m_MapNode = NULL;
	m_gc = NULL;
	m_ViewPosInfoText = NULL;
	m_EarthManip = NULL;
	m_ControlCanvas = NULL;
}

// 获取运行路径
std::string VRSim3D::SceneView::GetAppPath(void)
{
	char   path_buffer[_MAX_PATH];       
	char   drive[_MAX_DRIVE];       
	char   dir[_MAX_DIR]; 
	char   fname[_MAX_FNAME];       
	char   ext[_MAX_EXT]; 

	/*GetModuleFileName(AfxGetInstanceHandle(),path_buffer,_MAX_PATH*2); */
	///zqgao 这里暂时写死，获取DLL路径
	HINSTANCE hinstance = (HINSTANCE)GetModuleHandle("VRSim3DAPI.dll");
	GetModuleFileName(hinstance,path_buffer,_MAX_PATH);
	//GetCurrentDirectory(_MAX_PATH,drive); 
	_splitpath(path_buffer,   drive,   dir,   fname,   ext); 

	std::string   sPath(drive); 
	//sPath=drive; 
	sPath+=dir; 
	return   sPath;
}

// zqgao 初始化主相机
void VRSim3D::SceneView::initCamera(void)
{
	if(!m_gc.get())
		return;
	// 将窗口图形上下文传递给新建的摄像机，并设置摄像机的视口，透视矩阵等参数
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;

	camera->setGraphicsContext(m_gc.get());

	camera->setViewport(new osg::Viewport(m_gc->getTraits()->x, m_gc->getTraits()->y, 
		m_gc->getTraits()->width, m_gc->getTraits()->height));
	camera->setProjectionMatrixAsPerspective( 30.0f,
		(double)m_gc->getTraits()->width/(double)m_gc->getTraits()->height, 1.0f, 10000.0f );
	///dbs 针对球面场景，自动调整远近裁剪面，避免接近地面时切掉近处场景地面
	osg::ref_ptr<osgEarth::Util::AutoClipPlaneCullCallback> pAt = 
					new osgEarth::Util::AutoClipPlaneCullCallback(m_MapNode.get());
	camera->addCullCallback(pAt.get());
	///dbs 设置横纵比例
	double fovy, aspectRatio, z1, z2;
	camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);
	aspectRatio = double(m_gc->getTraits()->width)/double(m_gc->getTraits()->height);
	camera->setProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);

	m_Viewer->setCamera( camera.get() );
}

// zqgao 设置场景
void VRSim3D::SceneView::setupView(void)
{ 
	if(!m_Viewer.get())
		return; 
	//初始化相机
	initCamera();
	// 设置观察场景的视景器，将新的摄像机设置为场景的主摄像机
	// 这里我们还设置了视景器线程模型，由于我们是使用自建的线程来执行渲染，
	// 因此建议使用单线程的模型，以避免处理场景时出现问题
	m_Viewer->setThreadingModel( osgViewer::Viewer::SingleThreaded );
	// add some stock OSG handlers:
	m_Viewer->addEventHandler(new osgViewer::StatsHandler());
	m_Viewer->addEventHandler(new osgViewer::WindowSizeHandler());
	m_Viewer->addEventHandler(new osgViewer::ThreadingHandler());
	m_Viewer->addEventHandler(new osgGA::StateSetManipulator(m_Viewer->getCamera()->getOrCreateStateSet()));

	//m_viewer->addEventHandler(new osgGA::StateSetManipulator(view->getCamera()->getOrCreateStateSet()));
	m_Viewer->setKeyEventSetsDone(0);//注释掉esc
#if(0)
	m_ControlCanvas = osgEarth::Util::Controls::ControlCanvas::get(m_viewer.get(), true);
	m_ControlCanvas->setNodeMask(0x1);
	//星空
	//ChangeSkySet(false);
	//鼠标位置
	showMousePosition();
	//显示屏幕中心坐标
	makeViewPosInfoText(EarthManip,10, 10);
#endif 
	///zqgao 控件面板层 控件显示相当于HUDCamera，需要场景camera创建后才有意义
	m_ControlCanvas = osgEarth::Util::Controls::ControlCanvas::get(m_Viewer.get(), true);
	m_ControlCanvas->setNodeMask(0x1);
	m_ControlCanvas->setName("ControlCanvas");
	//LoadGlobe("E:/VR_SM3D/VR_SH/bin/3Ddata/startup.earth");
	///zqgao 保存基础事件
	m_BasicEvents = m_Viewer->getEventHandlers();
	m_Viewer->setSceneData(m_Root.get());
	m_Viewer->realize();
	_beginthread(&RenderThread, 0, m_Viewer.get());
}

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

bool VRSim3D::SceneView::LoadModel(const std::string& path)
{
	return false;
}

bool VRSim3D::SceneView::LoadGlobe(const std::string& path,bool isEarthManip /*= true*/)
{
	if(m_MapNode.get())
		return false;
	std::string fileName = osgDB::findDataFile(path, 0);
	osg::ref_ptr<osg::Node> globeNode;
	if(!fileName.empty())
		globeNode = osgDB::readNodeFile(fileName);
	m_MapNode = osgEarth::MapNode::findMapNode(globeNode.get());
	if (m_MapNode.valid())//("获取影像根节点失败！");
	{
		///m_Root->addChild(m_MapNode.get());
		m_CommandHandler->ChildCommand(m_Root.get(),m_MapNode.get(),CommandHandler::ADD);
		if(isEarthManip)
		{
			///zqgao 添加地球操控器
			setEarthManip(); 
			///zqgao 显示鼠标位置（与球面交点）
			showMousePosition();
			///zqgao 显示屏幕中心位置
			m_ViewPosInfoText = new ViewPosInfoText(m_EarthManip.get());
			osg::ref_ptr<osg::Node> node = m_ViewPosInfoText->makeViewPosInfoText(0,0,
											m_gc->getTraits()->width, m_gc->getTraits()->height);
			m_Root->addChild(node.get());
		}
	}
	return false;
}

void VRSim3D::SceneView::setEarthManip()
{   
	if(!m_EarthManip.get())
	{
		m_EarthManip = new osgEarth::Util::EarthManipulator();
		m_EarthManip->setIntersectTraversalMask(0xF0000001);
		m_EarthManip->getSettings()->setTetherMode(osgEarth::Util::EarthManipulator::TETHER_CENTER_AND_HEADING);
		m_EarthManip->getSettings()->setArcViewpointTransitions(true);
		double n = m_EarthManip->getSettings()->getMinDistance();
		m_EarthManip->getSettings()->setMinMaxDistance(n, 348062470);

		m_KeySwitchMatrixManipulator->addMatrixManipulator(m_MpNum++,"EarthManip",m_EarthManip.get());
	}

	int index = UtileMethod::getMPIndexFromSwichMP(m_KeySwitchMatrixManipulator.get(),"EarthManip");
	m_KeySwitchMatrixManipulator->selectMatrixManipulator(index);

	m_EarthManip->setHomeViewpoint( osgEarth::Viewpoint(osg::Vec3d(118,25,0), 0, -89,
		m_MapNode->getMap()->getProfile()->getSRS()->getEllipsoid()->getRadiusEquator()*3.0 ), 2); 

	m_EarthManip->setViewpoint(osgEarth::Viewpoint(osg::Vec3d(119,24,0), 0, -89,
		m_MapNode->getMap()->getProfile()->getSRS()->getEllipsoid()->getRadiusEquator()*3.0 ),2.0);
	//m_Viewer->setCameraManipulator(m_EarthManip.get());
}

void VRSim3D::SceneView::showMousePosition(bool isShow)
{
	if(m_MouseCoords.get())
		return;
	///dbs 首先检查变量参数合法性
	if ((!m_Viewer.get()) || (!m_MapNode.valid()))  return;

	///dbs 提取字体文件，并判断其合法性
	if (!m_AllFont.valid())   return;

	///dbs 创建用于显示鼠标实时位置的控件，并将其添加到面板中
	m_MouseCoords = new osgEarth::Util::Controls::LabelControl();
	m_MouseCoords->setFont(m_AllFont.get());
	m_MouseCoords->setFontSize(12);
	//mouseCoords->setForeColor()
	m_MouseCoords->setHorizAlign(Control::ALIGN_CENTER);
	m_MouseCoords->setVertAlign(Control::ALIGN_TOP);
	m_MouseCoords->setBackColor(0.0, 0.2, 0.2, 0.4);    
	m_MouseCoords->setSize(400, 50);
	m_MouseCoords->setMargin(8);
	m_MouseCoords->setVisible(isShow);
	m_ControlCanvas->addControl(m_MouseCoords.get());

	///dbs 鼠标位置实时显示
	osg::ref_ptr<MouseCoordsHandler> mMouseCoordsHandler = new MouseCoordsHandler(m_MouseCoords, m_MapNode);
	m_Viewer->addEventHandler(mMouseCoordsHandler.get());
}

void VRSim3D::SceneView::showMouseInfor( bool isShow )
{
	if(m_MouseCoords.get())
	{
		if(m_MouseCoords->visible() != isShow )
			m_MouseCoords->setVisible(isShow);
	}
}

bool VRSim3D::SceneView::RemoveGlobe()
{
	DelEventHandlers();
 	DelMouseInfor();
	DelViewPosInfoText();
	///zqgao 删除地球节点
	if(m_MapNode.get())
		m_CommandHandler->ChildCommand(m_Root.get(),m_MapNode.get(),CommandHandler::DEL);
	///zqgao 更换相机操控器,跟换为Trackball操控器
	int index = UtileMethod::getMPIndexFromSwichMP(m_KeySwitchMatrixManipulator.get(),"Trackball");
	m_KeySwitchMatrixManipulator->selectMatrixManipulator(index);
	///zqgao 释放地球节点
	m_MapNode.release();
	m_MapNode = NULL;

	return true;
}

void VRSim3D::SceneView::showViewPosInfoText( bool isShow /*= true*/ )
{
	if(!m_ViewPosInfoText.get())
		return;
	m_ViewPosInfoText->setVisible(isShow);
}

bool VRSim3D::SceneView::DelMouseInfor()
{
	if (!m_MouseCoords.get())
		return false;
#if(0)
	typedef std::list< osg::ref_ptr<osgGA::GUIEventHandler> > EventHandlers;
	EventHandlers eventHandlers = m_Viewer->getEventHandlers();
	EventHandlers::iterator itor = eventHandlers.begin();
	///zqgao 删除鼠标位置实时显示事件
	osg::ref_ptr<MouseCoordsHandler> mMouseCoordsHandler = NULL;
	while(itor != eventHandlers.end())
	{
		mMouseCoordsHandler = dynamic_cast<MouseCoordsHandler*>(itor->get());
		if(mMouseCoordsHandler.get())
		{
			m_Viewer->removeEventHandler(mMouseCoordsHandler.get());
			break;
		}
		itor++;
	}
#endif
	///zqgao 删除鼠标位置信息节点
	m_ControlCanvas->removeControl(m_MouseCoords.get());
	///zqgao 释放鼠标删除鼠标位置信息节点
	m_MouseCoords.release();
	m_MouseCoords = NULL;
	return true;
}

bool VRSim3D::SceneView::DelViewPosInfoText()
{
	if (!m_ViewPosInfoText.get())
		return false;
	osg::ref_ptr<osg::Node> inforNode = m_ViewPosInfoText->getInforNode();
	if(inforNode.get())
	{
		m_CommandHandler->ChildCommand(inforNode->getParent(0),inforNode.get(),CommandHandler::DEL);
		///zqgao 释放
		m_ViewPosInfoText.release();
		m_ViewPosInfoText = NULL;
	}
}

void VRSim3D::SceneView::DelEventHandlers()
{
	//typedef std::list< osg::ref_ptr<osgGA::GUIEventHandler> > EventHandlers;
	BASICEVENTS eventHandlers = m_Viewer->getEventHandlers();
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
			m_Viewer->removeEventHandler(itor->get());	
		}
		isBasic = false;
		basicitor = m_BasicEvents.begin();
		itor++;
	}
}

bool VRSim3D::SceneView::AddEventHandlers( osgGA::GUIEventHandler* event )
{
	BASICEVENTS eventHandlers = m_Viewer->getEventHandlers();
	BASICEVENTS::iterator itor = m_BasicEvents.begin();
	while(itor != m_BasicEvents.end())
	{
		if(event == itor->get())
			return false;
		itor++;
	}
	m_BasicEvents.push_back(event);
	return true;
}

void VRSim3D::SceneView::clear()
{
	 
}

void VRSim3D::SceneView::clearRoot()
{
	if(!m_Root.get())
		return;
	for (unsigned int index = 0;index<m_Root->getNumChildren();++index)
	{
		m_CommandHandler->ChildCommand(m_Root.get(),m_Root->getChild(index),CommandHandler::DEL);
	}
}
