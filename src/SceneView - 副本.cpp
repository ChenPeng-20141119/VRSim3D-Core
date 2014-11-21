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
	///zqgao �����Ļ���
	setGraphicsContext(hwnd,width,height,samples);
	///zqgao ���ó���
	setupView();
	return false;
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
	if (hwnd)
	{
		osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData( hwnd);
		traits->inheritedWindowData = windata;
	}
	m_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
}

// zqgao ��ʼ��
void VRSim3D::SceneView::init(void)
{

	m_Viewer = new osgViewer::Viewer();
	m_KeySwitchMatrixManipulator = new osgGA::KeySwitchMatrixManipulator();
	///zqgao �൱��char '0',�����ü����л��ٿ���
	m_MpNum = 48;
	m_KeySwitchMatrixManipulator->addMatrixManipulator(m_MpNum++,"Trackball",new osgGA::TrackballManipulator());
	m_KeySwitchMatrixManipulator->selectMatrixManipulator(0);
	m_Viewer->setCameraManipulator(m_KeySwitchMatrixManipulator.get());
	m_KeySwitchMatrixManipulator->setName("SwichMP");

	m_Root = new osg::Group();
	m_Root->setName("Root");
	/// ������ʱ����Ϊ��̬���Ա����Ӽ��ٽڵ�ʱ������
	m_Root->setDataVariance(osg::Node::DataVariance::DYNAMIC);
	
	///zqgao �豸��
	m_ModelRoot = new osg::Group();
	m_ModelRoot->setName("ModelRoot");
	m_Root->addChild(m_ModelRoot.get());

	m_Dir = GetAppPath()  + "3Ddata" + "/";
	///zqgao ����Ĭ������·�����������Ŀ¼
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

// ��ȡ����·��
std::string VRSim3D::SceneView::GetAppPath(void)
{
	char   path_buffer[_MAX_PATH];       
	char   drive[_MAX_DRIVE];       
	char   dir[_MAX_DIR]; 
	char   fname[_MAX_FNAME];       
	char   ext[_MAX_EXT]; 

	/*GetModuleFileName(AfxGetInstanceHandle(),path_buffer,_MAX_PATH*2); */
	///zqgao ������ʱд������ȡDLL·��
	HINSTANCE hinstance = (HINSTANCE)GetModuleHandle("VRSim3DAPI.dll");
	GetModuleFileName(hinstance,path_buffer,_MAX_PATH);
	//GetCurrentDirectory(_MAX_PATH,drive); 
	_splitpath(path_buffer,   drive,   dir,   fname,   ext); 

	std::string   sPath(drive); 
	//sPath=drive; 
	sPath+=dir; 
	return   sPath;
}

// zqgao ��ʼ�������
void VRSim3D::SceneView::initCamera(void)
{
	if(!m_gc.get())
		return;
	// ������ͼ�������Ĵ��ݸ��½������������������������ӿڣ�͸�Ӿ���Ȳ���
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;

	camera->setGraphicsContext(m_gc.get());

	camera->setViewport(new osg::Viewport(m_gc->getTraits()->x, m_gc->getTraits()->y, 
		m_gc->getTraits()->width, m_gc->getTraits()->height));
	camera->setProjectionMatrixAsPerspective( 30.0f,
		(double)m_gc->getTraits()->width/(double)m_gc->getTraits()->height, 1.0f, 10000.0f );
	///dbs ������泡�����Զ�����Զ���ü��棬����ӽ�����ʱ�е�������������
	osg::ref_ptr<osgEarth::Util::AutoClipPlaneCullCallback> pAt = 
					new osgEarth::Util::AutoClipPlaneCullCallback(m_MapNode.get());
	camera->addCullCallback(pAt.get());
	///dbs ���ú��ݱ���
	double fovy, aspectRatio, z1, z2;
	camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);
	aspectRatio = double(m_gc->getTraits()->width)/double(m_gc->getTraits()->height);
	camera->setProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);

	m_Viewer->setCamera( camera.get() );
}

// zqgao ���ó���
void VRSim3D::SceneView::setupView(void)
{ 
	if(!m_Viewer.get())
		return; 
	//��ʼ�����
	initCamera();
	// ���ù۲쳡�����Ӿ��������µ����������Ϊ�������������
	// �������ǻ��������Ӿ����߳�ģ�ͣ�����������ʹ���Խ����߳���ִ����Ⱦ��
	// ��˽���ʹ�õ��̵߳�ģ�ͣ��Ա��⴦����ʱ��������
	m_Viewer->setThreadingModel( osgViewer::Viewer::SingleThreaded );
	// add some stock OSG handlers:
	m_Viewer->addEventHandler(new osgViewer::StatsHandler());
	m_Viewer->addEventHandler(new osgViewer::WindowSizeHandler());
	m_Viewer->addEventHandler(new osgViewer::ThreadingHandler());
	m_Viewer->addEventHandler(new osgGA::StateSetManipulator(m_Viewer->getCamera()->getOrCreateStateSet()));

	//m_viewer->addEventHandler(new osgGA::StateSetManipulator(view->getCamera()->getOrCreateStateSet()));
	m_Viewer->setKeyEventSetsDone(0);//ע�͵�esc
#if(0)
	m_ControlCanvas = osgEarth::Util::Controls::ControlCanvas::get(m_viewer.get(), true);
	m_ControlCanvas->setNodeMask(0x1);
	//�ǿ�
	//ChangeSkySet(false);
	//���λ��
	showMousePosition();
	//��ʾ��Ļ��������
	makeViewPosInfoText(EarthManip,10, 10);
#endif 
	///zqgao �ؼ����� �ؼ���ʾ�൱��HUDCamera����Ҫ����camera�������������
	m_ControlCanvas = osgEarth::Util::Controls::ControlCanvas::get(m_Viewer.get(), true);
	m_ControlCanvas->setNodeMask(0x1);
	m_ControlCanvas->setName("ControlCanvas");
	//LoadGlobe("E:/VR_SM3D/VR_SH/bin/3Ddata/startup.earth");
	///zqgao ��������¼�
	m_BasicEvents = m_Viewer->getEventHandlers();
	m_Viewer->setSceneData(m_Root.get());
	m_Viewer->realize();
	_beginthread(&RenderThread, 0, m_Viewer.get());
}

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
	if (m_MapNode.valid())//("��ȡӰ����ڵ�ʧ�ܣ�");
	{
		///m_Root->addChild(m_MapNode.get());
		m_CommandHandler->ChildCommand(m_Root.get(),m_MapNode.get(),CommandHandler::ADD);
		if(isEarthManip)
		{
			///zqgao ��ӵ���ٿ���
			setEarthManip(); 
			///zqgao ��ʾ���λ�ã������潻�㣩
			showMousePosition();
			///zqgao ��ʾ��Ļ����λ��
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
	///dbs ���ȼ����������Ϸ���
	if ((!m_Viewer.get()) || (!m_MapNode.valid()))  return;

	///dbs ��ȡ�����ļ������ж���Ϸ���
	if (!m_AllFont.valid())   return;

	///dbs ����������ʾ���ʵʱλ�õĿؼ�����������ӵ������
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

	///dbs ���λ��ʵʱ��ʾ
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
	///zqgao ɾ������ڵ�
	if(m_MapNode.get())
		m_CommandHandler->ChildCommand(m_Root.get(),m_MapNode.get(),CommandHandler::DEL);
	///zqgao ��������ٿ���,����ΪTrackball�ٿ���
	int index = UtileMethod::getMPIndexFromSwichMP(m_KeySwitchMatrixManipulator.get(),"Trackball");
	m_KeySwitchMatrixManipulator->selectMatrixManipulator(index);
	///zqgao �ͷŵ���ڵ�
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
	///zqgao ɾ�����λ��ʵʱ��ʾ�¼�
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
	///zqgao ɾ�����λ����Ϣ�ڵ�
	m_ControlCanvas->removeControl(m_MouseCoords.get());
	///zqgao �ͷ����ɾ�����λ����Ϣ�ڵ�
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
		///zqgao �ͷ�
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
