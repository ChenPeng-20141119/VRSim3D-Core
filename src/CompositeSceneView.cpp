#include "CompositeSceneView.h"

using namespace VRSim3D;
#define GAP 0.5;

CompositeSceneView::CompositeSceneView(void)
{
	myinit();
}

CompositeSceneView::~CompositeSceneView(void)
{
	if(m_Viewer.get())
	{
		m_Viewer->stopThreading();
		m_Viewer->setDone(true);
		Sleep(100);
		delete m_Viewer;
	}
}

void VRSim3D::CompositeSceneView::myinit( void )
{
	m_Viewer = new osgViewer::CompositeViewer;
	///zqgao 相当于char '0',便于用键盘切换操控器
	m_MpNum = 48;
}

bool VRSim3D::CompositeSceneView::CreateScene( HWND hwnd,const float &width /*= 800.0f*/,const float &height /*= 600.0f*/,const unsigned int &samples /*= 16*/ )
{
	///zqgao 上下文环境
	setGraphicsContext(hwnd,width,height,samples);
	for (int index = 0;index <4;index++)
	{
		setupView(index);
	}
	m_Viewer->realize();
	_beginthread(&RenderThread, 0, m_Viewer.get());
}

void VRSim3D::CompositeSceneView::initCamera( void )
{
	
}

void VRSim3D::CompositeSceneView::setupView( const int &index )
{

	if(!m_gc.get())
		return;
	///zqgao 设置各个view
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
	m_Viewer->addView(view.get());
	view->getCamera()->setViewport(m_gc->getTraits()->x, m_gc->getTraits()->y, 
		m_gc->getTraits()->width, m_gc->getTraits()->height);
	view->getCamera()->setGraphicsContext(m_gc);
	/*view->getCamera()->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);*/

	///zqgao 添加默认相机操控器
	m_MpNum += index;
	setCmaraMP(view.get(),new osgGA::TrackballManipulator(),"Trackball");

#if 0
	///dbs 针对球面场景，自动调整远近裁剪面，避免接近地面时切掉近处场景地面
	AutoClipPlaneCullCallback* pAt = new AutoClipPlaneCullCallback(mMapNode);
	view->getCamera()->addCullCallback(pAt);
#endif
	
	view->addEventHandler(new osgViewer::StatsHandler());
	view->addEventHandler(new osgViewer::WindowSizeHandler());
	view->addEventHandler(new osgViewer::ThreadingHandler());
	view->addEventHandler(new osgGA::StateSetManipulator(view->getCamera()->getOrCreateStateSet()));
	//view->addEventHandler(new osgViewer::HelpHandler());

	///dbs 加入LOD系数调整处理器
	osg::ref_ptr<osgViewer::LODScaleHandler> pLODHandler = new osgViewer::LODScaleHandler;
	pLODHandler->setKeyEventIncreaseLODScale('+');
	pLODHandler->setKeyEventDecreaseLODScale('-');
	view->addEventHandler(pLODHandler.get());


	///dbs 设置横纵比例
	double fovy, aspectRatio, z1, z2;
	view->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);
	aspectRatio = double(m_gc->getTraits()->width)/double(m_gc->getTraits()->height);
	view->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);

}


void VRSim3D::CompositeSceneView::setCmaraMP( osgViewer::View* view,osgGA::CameraManipulator* cm,std::string name )
{
	m_KeySwitchMatrixManipulator = new osgGA::KeySwitchMatrixManipulator();
	m_KeySwitchMatrixManipulator->addMatrixManipulator(m_MpNum,name,cm);
	m_KeySwitchMatrixManipulator->selectMatrixManipulator(0);
	view->setCameraManipulator(m_KeySwitchMatrixManipulator.get());
	m_KeySwitchMatrixManipulator->setName("SwichMP");
}

void VRSim3D::CompositeSceneView::setCompostViewers( Style style /*= ST_AUTOMATIC*/ )
{
	if(m_Viewer->getNumViews()<1)
		return;
	partition(style);
}

void VRSim3D::CompositeSceneView::partition( Style style )
{
	float width = m_gc->getTraits()->width;
	float hight = m_gc->getTraits()->height;
	std::string num = "";
	std::string name = "";
	osg::ref_ptr<osgViewer::View> view = NULL;
	float roll = 0.0f;
	int viewerNum = m_Viewer->getNumViews();
	for (int i = 0;i<viewerNum;++i)
	{ 
		name = "View_" + UtileMethod::IntToStr(i);
		view = m_Viewer->getView(i);
		view->setName(name);
		//view->addEventHandler(new SelectEnvent(this));
		setCameraPPT(i,viewerNum,style);
	}
}

void VRSim3D::CompositeSceneView::setCameraPPT( int numview,int allview,Style style )
{
	float x,y,width,hight;
	if(allview == 1)
	{
		x = 0.0f;
		y = 0.0f;
		width = m_gc->getTraits()->width;
		hight = m_gc->getTraits()->height;
	}else
	{
		switch(style)
		{
		case ST_AUTOMATIC:
			{
				setViewPos(numview,allview,x,y,width,hight);
			}
			break;
		case ST_HORIZONTAL:
			{
				width = m_gc->getTraits()->width;
				hight = (m_gc->getTraits()->height - ((allview-1)*GAP))/(float)allview;
				x = 0.0f;
				y = numview*(hight + GAP);
			}
			break;
		case ST_VERTICAL:
			{
				width = (m_gc->getTraits()->width - ((allview-1)*GAP))/(float)allview;
				hight = m_gc->getTraits()->height;
				x = numview*(width + GAP);
				y = 0.0f;
			}
			break;
		default:
			break;
		}
	}

	m_Viewer->getView(numview)->getCamera()->setViewport(new osg::Viewport(x,y, width, hight));
	m_Viewer->getView(numview)->getCamera()->setGraphicsContext(m_gc.get());
}

void VRSim3D::CompositeSceneView::setViewPos( const int& numview,const int& allview,float& x, float& y, float& width, float& hight )
{
	int row = 0;
	int line = 0;
	width = 2*(m_gc->getTraits()->width - ((allview/2-1)*GAP))/(float)allview;
	hight = 2*(m_gc->getTraits()->height - ((allview/2-1)*GAP))/(float)allview;
	if(2 == allview)
	{
		width = (m_gc->getTraits()->width - ((allview-1)*GAP))/(float)allview;
		hight = m_gc->getTraits()->height;
		x = numview*(width + GAP);
		y = 0.0f;
	}else if (0 == allview %2)
	{
		line = numview%(allview/2);
		x = line*(width + GAP);
		row = numview/(allview/2);
		y = row*(hight + GAP);
	}
}

void VRSim3D::CompositeSceneView::RenderThread( void* ptr )
{
	osg::ref_ptr<osgViewer::CompositeViewer> view = (osgViewer::CompositeViewer*)(ptr);
	if (view )
	{
		while(!view->done())
		{
			view->frame();
		}
		_endthread();
	}
}
