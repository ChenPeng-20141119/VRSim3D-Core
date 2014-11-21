#ifdef _QTWIND
#include "SingleSceneView-QT.h"

using namespace VRSim3D;

SingleSceneViewQT::SingleSceneViewQT(int width, int height)
{
	
}

SingleSceneViewQT::~SingleSceneViewQT(void)
{
}


void VRSim3D::SingleSceneViewQT::keyPressEvent( QKeyEvent* event )
{
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(!window.valid())
		return;
	std::string key = std::string(event->text().toLocal8Bit().data());
	if(key == "+")
	{
		window->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KeySymbol::KEY_KP_Add);
	}
	else if (key == "-")
	{
		window->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KeySymbol::KEY_KP_Subtract);
	}
	else
		window->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data()) );
}

void VRSim3D::SingleSceneViewQT::keyReleaseEvent( QKeyEvent* event )
{
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(window.valid())
		window->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data()) );
}

void VRSim3D::SingleSceneViewQT::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsScene::mousePressEvent(event);
	int button = 0;

	switch ( event->button() )
	{
	case (Qt::LeftButton):
		button = 1;
		break;
	case (Qt::MidButton):
		button = 2;
		break;
	case (Qt::RightButton):
		button = 3;
		break;
	case (Qt::NoButton):
		button = 0;
		break;
	default:
		button = 0;
		break;
	}
	QGraphicsScene::mouseMoveEvent(event);
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(window.valid())
		window->getEventQueue()->mouseButtonPress(event->scenePos().x(), event->scenePos().y(), button);
}

void VRSim3D::SingleSceneViewQT::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	int button = 0;

	switch ( event->button() )
	{
	case (Qt::LeftButton):
		button = 1;
		break;
	case (Qt::MidButton):
		button = 2;
		break;
	case (Qt::RightButton):
		button = 3;
		break;
	case (Qt::NoButton):
		button = 0;
		break;
	default:
		button = 0;
		break;
	}
	QGraphicsScene::mouseMoveEvent(event);
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(window.valid())
		window->getEventQueue()->mouseButtonRelease(event->scenePos().x(), event->scenePos().y(), button);
}

void VRSim3D::SingleSceneViewQT::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsScene::mouseMoveEvent(event);
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(window.valid())
		window->getEventQueue()->mouseMotion(event->scenePos().x(), event->scenePos().y());
}

void VRSim3D::SingleSceneViewQT::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	QGraphicsScene::wheelEvent(event);
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(window.valid())
		window->getEventQueue()->
		mouseScroll(event->delta()>0 ? 
		osgGA::GUIEventAdapter::SCROLL_UP : 
	osgGA::GUIEventAdapter::SCROLL_DOWN );
}

void VRSim3D::SingleSceneViewQT::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	int button = 0;
	switch(event->button())
	{
	case(Qt::LeftButton): button = 1; break;
	case(Qt::MidButton): button = 2; break;
	case(Qt::RightButton): button = 3; break;
	case(Qt::NoButton): button = 0; break;
	default: button = 0; break;
	}
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(window.valid())
		window->getEventQueue()->mouseDoubleButtonPress(event->scenePos().x(), event->scenePos().y(),button);
}

void VRSim3D::SingleSceneViewQT::drawBackground( QPainter *painter, const QRectF &rect )
{
	osg::ref_ptr<osgViewer::Viewer> view = dynamic_cast<osgViewer::Viewer*>(getViewer());
	if(view.valid() && !view->done())
		view->frame();
}

void VRSim3D::SingleSceneViewQT::resize( int width, int height )
{
	osg::ref_ptr<osgViewer::GraphicsWindow> window = dynamic_cast<osgViewer::GraphicsWindow*>(getGraphicsContext());
	if(!window.valid())
		return;
	window->getEventQueue()->windowResize(0, 0, width, height);
	window->resized(0, 0, width, height);
}

bool VRSim3D::SingleSceneViewQT::CreateScene( HWND hwnd,const float &width /*= 800.0f*/,const float &height /*= 600.0f*/,const unsigned int &samples /*= 16*/ )
{
	///zqgao 上下文环境
	setGraphicsContext(hwnd,width,height,samples);
	///zqgao 设置场景
	setupView();

	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(10);

	return false;
}
#endif
