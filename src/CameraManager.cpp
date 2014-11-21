#include "CameraManager.h"

#include "osg/LineSegment"
#include "osgUtil/IntersectVisitor"
#include <math.h>

using namespace VRSim3D;
CameraManager::CameraManager(void)
{
	m_vPosition = osg::Vec3(-100,-112,15);
	m_vRotation = osg::Vec3(osg::PI_2,0,0);
	m_vStep = 1.0f;
	m_vRotateStep = 0.1f;
	m_vScollStep = 14;
	m_bRightDown = false;
	m_node = NULL;
	m_isMove = false;

	
	m_KeySymbol.KEY_BACK = false;
	m_KeySymbol.KEY_FRONT = false;
	m_KeySymbol.KEY_LEFT = false;
	m_KeySymbol.KEY_RIGHT = false;
}

CameraManager::~CameraManager(void)
{
}

void CameraManager::setByMatrix( const osg::Matrixd& matrix )
{

}

osg::Matrixd CameraManager::getMatrix() const
{
	osg::Matrixd mat;
	mat.makeTranslate(m_vPosition);
	//先旋转再平移
	return osg::Matrixd::rotate(m_vRotation[0],osg::X_AXIS,
								m_vRotation[1],osg::Y_AXIS,
								m_vRotation[2],osg::Z_AXIS)*mat;
}

void CameraManager::setByInverseMatrix( const osg::Matrixd& matrix )
{
	
}

osg::Matrixd CameraManager::getInverseMatrix() const
{
	osg::Matrixd mat;
	mat.makeTranslate(m_vPosition);
	return osg::Matrixd::inverse(         
						osg::Matrixd::rotate(m_vRotation[0],osg::X_AXIS,
											 m_vRotation[1],osg::Y_AXIS,
											 m_vRotation[2],osg::Z_AXIS)*mat);
}

bool CameraManager::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us )
{
	osgViewer::View* viewer = dynamic_cast<osgViewer::Viewer*>(&us);
	if(viewer)
	osg::Camera* camera = viewer->getCamera();
	switch(ea.getEventType())
	{
		//沿个方向移动距离
		float x,y,z;
	case osgGA::GUIEventAdapter::KEYDOWN:
		{
			//前
			if (ea.getKey() == 'w' || ea.getKey() == 'W')
			{
				m_KeySymbol.KEY_FRONT = true;
			}else
			if (ea.getKey() == 's' || ea.getKey() == 'S')
			{
				m_KeySymbol.KEY_BACK = true;
			}else
			if (ea.getKey() == 'a' || ea.getKey() == 'A')
			{
				m_KeySymbol.KEY_LEFT = true;
			}else
			if (ea.getKey() == 'd' || ea.getKey() == 'D')
			{
				m_KeySymbol.KEY_RIGHT = true;
			}else
				if(ea.getKey() == '='||ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Up)
			{
				setSetUp(1.0f);
			}else
			if (ea.getKey() == '-'||ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Down)
			{
				setSetUp(-1.0f);
			}else
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Home)
			{
				changePosition(osg::Vec3(0,0,1.0));
			}else
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_End)
			{
				changePosition(osg::Vec3(0,0,-1.0));
			}
		}
		break;
	case osgGA::GUIEventAdapter::KEYUP:
		{
			//m_isMove = false;
			if(ea.getKey() == 'w' || ea.getKey() == 'W')
				m_KeySymbol.KEY_FRONT = false;
			if(ea.getKey() == 's' || ea.getKey() == 'S')
				m_KeySymbol.KEY_BACK = false;
			if(ea.getKey() == 'a' || ea.getKey() == 'A')
				m_KeySymbol.KEY_LEFT = false;
			if(ea.getKey() == 'd' || ea.getKey() == 'D')
				m_KeySymbol.KEY_RIGHT = false;
		} 
		break;
	case osgGA::GUIEventAdapter::PUSH:
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				m_iLeftX = ea.getX();
				m_iLeftY = ea.getY();
				m_bRightDown = true;
			}
			return false;
		}
		break;
	case  osgGA::GUIEventAdapter::DRAG:
		{
			if (m_bRightDown)
			{
				static float tempX = 0.0f;
				static float tempY = 0.0f;
				float delX = ea.getX() - m_iLeftX;
				float delY = ea.getY() - m_iLeftY;
				m_vRotation[2] -= osg::DegreesToRadians(m_vRotateStep*delX);
				m_vRotation[0] += osg::DegreesToRadians(m_vRotateStep*delY);
				if (m_vRotation[0] <= 0)
				{
					m_vRotation[0] = 0; 
				}
				if (m_vRotation[0] >= osg::PI)
				{
					m_vRotation[0] = osg::PI;
				}
				m_iLeftX = ea.getX();
				m_iLeftY = ea.getY();
				tempX = delX;
				tempY = delY;
			}
		}
		break;
	case osgGA::GUIEventAdapter::RELEASE:
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				m_bRightDown = false;
			}
		}
		break;
	case  osgGA::GUIEventAdapter::SCROLL:
		{
			if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
			{
				computeChange(x,y,z,CHANGEBY_MOUSE);
				changePosition(osg::Vec3(x,y,z));
				//changePosition(osg::Vec3(0,0,2.0));
			}
			if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
			{
				computeChange(x,y,z,CHANGEBY_MOUSE);
				changePosition(osg::Vec3(-x,-y,-z));
			}
		}
		break;
	case osgGA::GUIEventAdapter::FRAME:
		{
			move();
		}
	default:
		break;
	}
	return false;
}

bool CameraManager::changePosition( osg::Vec3 delta )
{
	if(!Ishold(delta))
	m_vPosition += delta;
	return true;
}

void CameraManager::setSetUp( float data_setup )
{
	m_vStep += data_setup;
	if(0 >= m_vStep)
		m_vStep = 0;
}

void CameraManager::computeChange( float &x,float &y,float &z,ChangeBy changeby )
 {
	float step = 1.0;
	switch(changeby)
	{
	case CHANGEBY_KEY:
		step = m_vStep;
		break;
	case CHANGEBY_MOUSE:
		step = m_vScollStep;
		break;
	}
	if((m_KeySymbol.KEY_FRONT&&(m_KeySymbol.KEY_LEFT||m_KeySymbol.KEY_RIGHT))||
		(m_KeySymbol.KEY_BACK&&(m_KeySymbol.KEY_LEFT||m_KeySymbol.KEY_RIGHT)))
		step *= sinf(osg::PI_4);
	z = -step*sinf(osg::PI_2 + m_vRotation[0]);
	y = step*sinf(osg::PI_2  + m_vRotation[2]);
	x = step*cosf(osg::PI_2 + m_vRotation[2]);
}

void CameraManager::setModle( osg::Node *node )
{
	m_node = node;
}

bool CameraManager::Ishold( osg::Vec3 detla )
{
	if(!m_node)
		return false;
	osg::Vec3 newPos = m_vPosition + detla;
	osgUtil::IntersectVisitor iv;
	osg::ref_ptr<osg::LineSegment> line = new osg::LineSegment(newPos,m_vPosition);
	iv.addLineSegment(line.get());
	m_node->accept(iv);
	if (!iv.hits())
	{
		return false;
	}
	return true;
}

void CameraManager::move()
{
	/*if(!m_isMove)
		return;*/
	//沿个方向移动距离
	float x,y,z;
	computeChange(x,y,z);
	if(m_KeySymbol.KEY_FRONT)
		changePosition(osg::Vec3(x,y,z));
	if(m_KeySymbol.KEY_BACK)
		changePosition(osg::Vec3(-x,-y,-z));
	if(m_KeySymbol.KEY_LEFT)
		changePosition(osg::Vec3(-y,x,0));
	if(m_KeySymbol.KEY_RIGHT)
		changePosition(osg::Vec3(y,-x,0));
}
