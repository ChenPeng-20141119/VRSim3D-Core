#include "SingleSceneView.h"

#include "osgGA/KeySwitchMatrixManipulator"
#include "osgGA/StateSetManipulator"
#include "osgGA/GUIEventHandler"
#include "osgGA/TrackballManipulator"

#include "osgViewer/ViewerEventHandlers"

#include <process.h>

using namespace VRSim3D;

SingleSceneView::SingleSceneView(void)
{
	myinit();
}

SingleSceneView::~SingleSceneView(void)
{
	if(m_Viewer.get())
	{
		m_Viewer->stopThreading();
		m_Viewer->setDone(true);
		Sleep(100);
		delete m_Viewer;
	}
}

bool VRSim3D::SingleSceneView::CreateScene( HWND hwnd,const float &width /*= 800.0f*/,const float &height /*= 600.0f*/,const unsigned int &samples /*= 16*/ )
{
	///zqgao �����Ļ���
	setGraphicsContext(hwnd,width,height,samples);
	///zqgao ���ó���
	setupView();

	_beginthread(&RenderThread, 0, m_Viewer.get());
	return false;
}

void VRSim3D::SingleSceneView::myinit( void )
{
	m_Viewer = new osgViewer::Viewer();

	m_KeySwitchMatrixManipulator = new osgGA::KeySwitchMatrixManipulator();
	///zqgao �൱��char '0',�����ü����л��ٿ���
	m_MpNum = 48;
	m_KeySwitchMatrixManipulator->addMatrixManipulator(m_MpNum++,"Trackball",new osgGA::TrackballManipulator());
	m_KeySwitchMatrixManipulator->selectMatrixManipulator(0);
	m_Viewer->setCameraManipulator(m_KeySwitchMatrixManipulator.get());
	m_KeySwitchMatrixManipulator->setName("SwichMP");
}

void VRSim3D::SingleSceneView::setupView( void )
{
	if(!m_Viewer.get())
		return; 
	//��ʼ�����
	initCamera();
	// ���ù۲쳡�����Ӿ��������µ����������Ϊ�������������
	// �������ǻ��������Ӿ����߳�ģ�ͣ�����������ʹ���Խ����߳���ִ����Ⱦ��
	// ��˽���ʹ�õ��̵߳�ģ�ͣ��Ա��⴦����ʱ��������
	m_Viewer->setThreadingModel( osgViewer::Viewer::SingleThreaded );
	//zqgao ����"s",��ʾ������Ϣ
	m_Viewer->addEventHandler(new osgViewer::StatsHandler());
	m_Viewer->addEventHandler(new osgViewer::WindowSizeHandler());
	m_Viewer->addEventHandler(new osgViewer::ThreadingHandler());
	m_Viewer->addEventHandler(new osgViewer::LODScaleHandler());
	m_Viewer->addEventHandler(new osgGA::StateSetManipulator(m_Viewer->getCamera()->getOrCreateStateSet()));
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
	/*m_ControlCanvas = osgEarth::Util::Controls::ControlCanvas::get(m_Viewer.get(), true);
	m_ControlCanvas->setNodeMask(0x1);
	m_ControlCanvas->setName("ControlCanvas");*/
	//LoadGlobe("E:/VR_SM3D/VR_SH/bin/3Ddata/startup.earth");
	/*///zqgao ��������¼�
	m_BasicEvents = m_Viewer->getEventHandlers();*/
	m_Viewer->setSceneData(m_Root.get());
	m_Viewer->realize();
}

void VRSim3D::SingleSceneView::initCamera( void )
{

	
	// ������ͼ�������Ĵ��ݸ��½������������������������ӿڣ�͸�Ӿ���Ȳ���
	//osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	osg::ref_ptr<osg::Camera> camera = m_Viewer->getCamera();
	camera->setName("MainCamera");
	camera->setClearColor(osg::Vec4(0.0,0.0,0.0,1));
	float x,y,width,height;
#ifdef _QTWIND
	if(!m_gWindow.valid())
		return;
	camera->setGraphicsContext(m_gWindow.get());
	x		  = m_gWindow->getTraits()->x;
	y		  = m_gWindow->getTraits()->y;
	width = m_gWindow->getTraits()->width;
	height = m_gWindow->getTraits()->height;
#else
	if(!m_gc.valid())
		return;
	camera->setGraphicsContext(m_gc.get());
	x		  = m_gc->getTraits()->x;
	y		  = m_gc->getTraits()->y;
	width = m_gc->getTraits()->width;
	height = m_gc->getTraits()->height;
#endif
	
	camera->setViewport(new osg::Viewport(x, y, width, height));
	camera->setProjectionMatrixAsPerspective( 30.0f,
		(double)width/(double)height, 1.0f, 10000.0f );
	///dbs ���ú��ݱ���
	double fovy, aspectRatio, z1, z2;
	camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);
	aspectRatio = double(width)/double(height);
	camera->setProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);

	camera->setNearFarRatio(0.00002);
	camera->setSmallFeatureCullingPixelSize(-1.0f);
	//m_Viewer->setCamera( camera.get() );
}

void VRSim3D::SingleSceneView::RenderThread( void* ptr )
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

void VRSim3D::SingleSceneView::SaveBasicEvents()
{
	m_BasicEvents = m_Viewer->getEventHandlers();
}

osgViewer::ViewerBase* VRSim3D::SingleSceneView::getViewer()
{
	return m_Viewer.get();
}
