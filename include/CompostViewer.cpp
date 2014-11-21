#include "StdAfx.h"
#include <stdlib.h>
#include "CompostViewer.h"
#include "UtileMethod.h"

#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>

#define GAP (0.5f)
CompostViewer::CompostViewer(void)
{
	m_compositeViewer = new osgViewer::CompositeViewer();
	m_isFullScene = false;
	m_gc = NULL;
	m_num = 1000;
}

CompostViewer::~CompostViewer(void)
{
}

void CompostViewer::setCompostViewers( int viewerNum,osg::Node* scene,Style style /*= ST_AUTOMATIC*/ )
{
	osg::ref_ptr<osg::GraphicsContext> gc = setGraphicsContext();
	if(!gc)
		return;
	m_gc = gc;
	partition(viewerNum,gc.get(),scene,style);

	m_compositeViewer->setKeyEventSetsDone('1');
	//m_compositeViewer->addEventHandler(new SelectEnvent(this));
}

void CompostViewer::setCompostViewers( osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style /*= ST_AUTOMATIC*/ )
{
	if(!viewer || viewer->getNumViews()<1 ||!gc)
		return;
	m_gc = gc;
	partition(viewer,gc,style);

}

osg::GraphicsContext* CompostViewer::setGraphicsContext()
{
	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi) 
	{ 
		osg::notify(osg::NOTICE)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
		return NULL;
	}

	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 100;
	traits->y = 100;
	traits->width = 1000;
	traits->height = 800;
	/*traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;*/
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	////创建图形环境特性
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (gc.valid())
	{
		osg::notify(osg::INFO)<<"  GraphicsWindow has been created successfully."<<std::endl;

		// need to ensure that the window is cleared make sure that the complete window is set the correct colour
		// rather than just the parts of the window that are under the camera's viewports
		gc->setClearColor(osg::Vec4f(0.0f,0.0f,0.0f,1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE)<<"  GraphicsWindow has not been created successfully."<<std::endl;
		return NULL;
	}

	return gc.release();

}

void CompostViewer::setVertical( int viewerNum,osg::GraphicsContext* gc,osg::Node* scene )
{
	if(!m_compositeViewer || !gc)
		return;
	float width = gc->getTraits()->width;
	float hight = gc->getTraits()->height;
	std::string num = "";
	std::string name = "View_";
	osg::ref_ptr<osgViewer::View> view = NULL;
	float roll = 0.0f;
	for (int i = 0;i<viewerNum;++i)
	{ 
		name += UtileMethod::IntToStr(i);
		view = new osgViewer::View;
		view->setName(name);
		m_compositeViewer->addView(view.get());
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(scene);
		if(!mt)
		{
			mt = new osg::MatrixTransform();
			mt->addChild(scene);
		}
		//旋转，实现不同视图不同角度观看
		roll = (float)i/(float)viewerNum;
		mt->setMatrix(osg::Matrix::rotate(osg::PI*2*roll,osg::Z_AXIS));
		view->setSceneData(mt.get());
		setCameraPPT(i,viewerNum,gc,ST_VERTICAL);
	}
}

void CompostViewer::setHorizontal( int viewerNum,osg::GraphicsContext* gc,osg::Node* scene )
{

}

void CompostViewer::setCameraPPT( int numview,int allview,osg::GraphicsContext* gc,Style style /*= ST_AUTOMATIC*/ )
{
	/*if(m_compositeViewer->getNumViews() <= numview+1 || numview >= allview)
		return;*/
	float x,y,width,hight;
	if(allview == 1)
	{
		x = 0.0f;
		y = 0.0f;
		width = gc->getTraits()->width;
		hight = gc->getTraits()->height;
	}else
	{
		switch(style)
		{
		case ST_AUTOMATIC:
			{
				setViewPos(gc,numview,allview,x,y,width,hight);
			}
			break;
		case ST_HORIZONTAL:
			{
				width = gc->getTraits()->width;
				hight = (gc->getTraits()->height - ((allview-1)*GAP))/(float)allview;
				x = 0.0f;
				y = numview*(hight + GAP);
			}
			break;
		case ST_VERTICAL:
			{
				width = (gc->getTraits()->width - ((allview-1)*GAP))/(float)allview;
				hight = gc->getTraits()->height;
				x = numview*(width + GAP);
				y = 0.0f;
			}
			break;
		default:
			break;
		}
	}

	m_compositeViewer->getView(numview)->getCamera()->setViewport(new osg::Viewport(x,y, width, hight));
	m_compositeViewer->getView(numview)->getCamera()->setGraphicsContext(gc);
	/*m_compositeViewer->getView(numview)->setCameraManipulator(new osgGA::TrackballManipulator);*/
}

void CompostViewer::setCameraPPT( int numview,osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style /*= ST_AUTOMATIC*/ )
{
	int allview = viewer->getNumViews();
	if(numview >= allview)
		return;
	float x,y,width,hight;
	if(allview == 1)
	{
		x = 0.0f;
		y = 0.0f;
		width = gc->getTraits()->width;
		hight = gc->getTraits()->height;
	}else
	{
		switch(style)
		{
		case ST_AUTOMATIC:
			{
				setViewPos(gc,numview,allview,x,y,width,hight);
			}
			break;
		case ST_HORIZONTAL:
			{
				width = gc->getTraits()->width;
				hight = (gc->getTraits()->height - ((allview-1)*GAP))/(float)allview;
				x = 0.0f;
				y = numview*(hight + GAP);
			}
			break;
		case ST_VERTICAL:
			{
				width = (gc->getTraits()->width - ((allview-1)*GAP))/(float)allview;
				hight = gc->getTraits()->height;
				x = numview*(width + GAP);
				y = 0.0f;
			}
			break;
		default:
			break;
		}
	}

	viewer->getView(numview)->getCamera()->setViewport(new osg::Viewport(x,y, width, hight));
	viewer->getView(numview)->getCamera()->setGraphicsContext(gc);
	viewer->getView(numview)->setCameraManipulator(new osgGA::TrackballManipulator);
}

void CompostViewer::setViewPos( osg::GraphicsContext* gc,const int& numview,const int& allview,
								float& x,float& y,float& width,float& hight )
{
	int row = 0;
	int line = 0;
	width = 2*(gc->getTraits()->width - ((allview/2-1)*GAP))/(float)allview;
	hight = 2*(gc->getTraits()->height - ((allview/2-1)*GAP))/(float)allview;
	if(2 == allview)
	{
		width = (gc->getTraits()->width - ((allview-1)*GAP))/(float)allview;
		hight = gc->getTraits()->height;
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

void CompostViewer::partition( int viewerNum,osg::GraphicsContext* gc,osg::Node* scene,Style style /*= ST_AUTOMATIC*/ )
{
	if(!m_compositeViewer || !gc)
		return;
	float width = gc->getTraits()->width;
	float hight = gc->getTraits()->height;
	std::string num = "";
	std::string name = "";
	osg::ref_ptr<osgViewer::View> view = NULL;
	float roll = 0.0f;
	for (int i = 0;i<viewerNum;++i)
	{ 
		name = "View_" + UtileMethod::IntToStr(i);
		view = new osgViewer::View;
		view->setName(name);
		m_compositeViewer->addView(view.get());
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*>(scene);
		if(!mt)
		{
			mt = new osg::MatrixTransform();
			mt->addChild(scene);
		}
		//旋转，实现不同视图不同角度观看
		roll = (float)i/(float)viewerNum;
		mt->setMatrix(osg::Matrix::rotate(osg::PI*2*roll,osg::Z_AXIS));
		view->setSceneData(mt.get());
		//view->addEventHandler(new SelectEnvent(this));
		setCameraPPT(i,viewerNum,gc,style);
	}
}

void CompostViewer::partition( osgViewer::CompositeViewer* viewer,osg::GraphicsContext* gc,Style style /*= ST_AUTOMATIC*/ )
{
	m_compositeViewer = viewer;
	if(!m_compositeViewer || !gc)
		return;
	float width = gc->getTraits()->width;
	float hight = gc->getTraits()->height;
	std::string num = "";
	std::string name = "";
	osg::ref_ptr<osgViewer::View> view = NULL;
	float roll = 0.0f;
	int viewerNum = viewer->getNumViews();
	for (int i = 0;i<viewerNum;++i)
	{ 
		name = "View_" + UtileMethod::IntToStr(i);
		view = viewer->getView(i);
		view->setName(name);
		//view->addEventHandler(new SelectEnvent(this));
		setCameraPPT(i,viewerNum,gc,style);
	}
}

void CompostViewer::setFullScene( osgViewer::View* view )
{
	if(!m_isFullScene)
	{
		/*for (size_t i = 0;i<m_compositeViewer->getNumViews();++i)
		{
			if(view == m_compositeViewer->getView(i))
				view->setUpViewOnSingleScreen(0);
		}*/
		//m_viewPort.clear();
		//m_viewPort.push_back(view->getCamera()->getViewport()->x());
		//m_viewPort.push_back(view->getCamera()->getViewport()->y());
		//m_viewPort.push_back(view->getCamera()->getViewport()->width());
		//m_viewPort.push_back(view->getCamera()->getViewport()->height());
		saveNowViewPos(view);
		view->getCamera()->getViewport()->setViewport(
								0,
								0,
								m_gc->getTraits()->width,
								m_gc->getTraits()->height);
		//view->setUpViewAcrossAllScreens();
		////view->setUpViewAcrossAllScreens();
	}else
	{
		//view->getCamera()->setViewport(m_Traits->x,m_Traits->y,m_Traits->width,m_Traits->height);
		if(m_viewPort.size() == 4)
		{
			float x = (float)(m_gc->getTraits()->width)*m_viewPort[0];
			float y = (float)m_gc->getTraits()->height*m_viewPort[1];
			float width = (float)m_gc->getTraits()->width*m_viewPort[2];
			float height = (float)m_gc->getTraits()->height*m_viewPort[3];
			view->getCamera()->getViewport()->setViewport(x,y,width,height);
		}
	}
	m_isFullScene = !m_isFullScene;
	setViewsState(view);
}

void CompostViewer::setViewsState( osgViewer::View* view )
{
	for(unsigned int i = 0;i<m_compositeViewer->getNumViews();++i)
	{
		osg::ref_ptr<osgViewer::View> vi = m_compositeViewer->getView(i);
		if(m_isFullScene)
		{
			if(view == vi)
			{
				vi->getCamera()->setRenderOrder(osg::Camera::POST_RENDER);
			}else
			{
				vi->getCamera()->setRenderOrder(osg::Camera::PRE_RENDER);
				vi->getCamera()->setAllowEventFocus(false);
			}
		}else
		{
			vi->getCamera()->setRenderOrder(osg::Camera::POST_RENDER);
			vi->getCamera()->setAllowEventFocus(true);
		}
	}
}

void CompostViewer::saveNowViewPos(osgViewer::View* view)
{
	m_viewPort.clear();
	float x = (float)view->getCamera()->getViewport()->x()/(float)m_gc->getTraits()->width;
	float y = (float)view->getCamera()->getViewport()->y()/(float)m_gc->getTraits()->height;
	float width = (float)view->getCamera()->getViewport()->width()/(float)m_gc->getTraits()->width;
	float height = (float)view->getCamera()->getViewport()->height()/(float)m_gc->getTraits()->height;
	m_viewPort.push_back(x);
	m_viewPort.push_back(y);
	m_viewPort.push_back(width);
	m_viewPort.push_back(height);
}

void CompostViewer::setSceneFullOrPartition( unsigned int num )
{
	if(m_compositeViewer->getNumViews()<=num)
		return;
	osg::ref_ptr<osgViewer::View> view = m_compositeViewer->getView(num);
	if(!view)
		return;
	if (m_num != num && m_isFullScene)
	{
		//如果与以全屏的view不是一个，先关闭已全屏的view
		setFullScene(m_compositeViewer->getView(m_num));
		m_isFullScene = false;
	}
	setFullScene(view);
	m_num = num;
}

void CompostViewer::setClicksetSceneFullOrPartition( bool isClick )
{
	//if(m_compositeViewer->getNumViews()>1)
	//{
	//	osgViewer::View::EventHandlers::iterator it;
	//	osg::ref_ptr<osgViewer::View> view = NULL;
	//	for(unsigned int i = 0;i<m_compositeViewer->getNumViews();++i)
	//	{
	//		view = m_compositeViewer->getView(i);
	//		if(view)
	//		{
	//			if(isClick)
	//			{
	//				view->addEventHandler(new SelectEnvent(this));
	//			}else
	//			{
	//				it = view->getEventHandlers().begin();
	//				while (it != view->getEventHandlers().end())
	//				{
	//					SelectEnvent* selectEnvent = dynamic_cast<SelectEnvent*>(it->get());
	//					if (selectEnvent)
	//					{
	//						view->removeEventHandler(selectEnvent);
	//						break;
	//					}
	//					++it;
	//				}
	//			}
	//		}
	//	}
	//}
}

//bool CompostViewer::SelectEnvent::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
//{
//	//if(!m_compositeViewer)
//	//	return false;
//	//osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*>(&aa);
//	//if(!view)
//	//	return false;
//	// switch(ea.getEventType())
//	// {
//	// case osgGA::GUIEventAdapter::DOUBLECLICK:
//	//	 if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
//	//	 {
//	//		m_compositeViewer->setFullScene(view);
//	//	 }
//	// }
//	 return false;
//}
