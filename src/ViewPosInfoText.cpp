#include "ViewPosInfoText.h"

#include "CommonFunctions.h"
#include "UtileMethod.h"

#include <stdio.h>

using namespace VRSim3D;
VRSim3D::ViewPosInfoText::ViewPosInfoText( osgEarth::Util::EarthManipulator* EarthManip )
{
	m_EarthManip = EarthManip;
	m_InforNode	 = NULL;
	m_Visible	 = true;
}

ViewPosInfoText::~ViewPosInfoText(void)
{
	if(m_EarthManip.get())
	{
		m_EarthManip.release();
		m_EarthManip = NULL;
	}
	if(m_InforNode.get())
	{
		m_InforNode.release();
		m_InforNode = NULL;
	}
}

osg::Node* VRSim3D::ViewPosInfoText::makeViewPosInfoText( const float &x,const float &y, const float &width,const float &height )
{
	if(m_InforNode.get())
		return NULL;
	osg::ref_ptr<osg::Camera> hudCamera = NULL;

#ifndef _QTWIND
	hudCamera = VRSim3D::createHUDCamera(x,x+width,y,y+height);
#else
	hudCamera = VRSim3D::createHUDCamera(x,x+width,y,y+height);
#endif

	hudCamera->setName("InforNode");

	osg::ref_ptr<osg::Geode> viewPosInfoText = new osg::Geode();
	hudCamera->addChild(viewPosInfoText.get());

	osg::ref_ptr<osgText::Text>  mInforText = VRSim3D::createText(osg::Vec3(x,y/* + 20*/,0),"Hello World",20);
	mInforText->setColor(osg::Vec4(0, 1, 0, 1));

	/// ����������Ϣ�ĸ��»ص�����
	mInforText->setDrawCallback(new ValueTextDrawCallback(m_EarthManip.get()));

	/// ������Ϣ��ʾ�ڵ�����ԣ����رչ��պ���Ȳ���
	osg::StateSet* stateset = viewPosInfoText->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	/// ���ı���ӵ������б���
	viewPosInfoText->addDrawable(mInforText.get());
	m_InforNode = hudCamera.get();

	return hudCamera.release();
}

void VRSim3D::ViewPosInfoText::setVisible( bool visible )
{
	if(m_InforNode.get())
	{
		if(m_Visible != visible)
		{
			m_InforNode->setNodeMask(visible);
			m_Visible = visible;
		}
	}
}

VRSim3D::ViewPosInfoText::ValueTextDrawCallback::ValueTextDrawCallback( osgEarth::Util::EarthManipulator *pManp )
{
	m_EarthManp = pManp;
}

void VRSim3D::ViewPosInfoText::ValueTextDrawCallback::drawImplementation( osg::RenderInfo& renderInfo, const osg::Drawable* drawable ) const
{
	/// �õ����ֱ�ע��ʵ��ָ��
	osgText::Text* text = (osgText::Text*)(drawable);
	if(!text)
		return;

	/// ��ȡ��ǰ�������۲�������������ӡ���
	osgEarth::Util::Viewpoint vp = m_EarthManp->getViewpoint();

	char buffer[_MAX_PATH];
	sprintf(buffer,"����:%6.2f  γ��:%6.2f  �߶�:%10.1f ����:%5.2f ��б:%5.2f", 
		vp.x(), vp.y(), vp.getRange(), vp.getHeading(), vp.getPitch());
	std::string tempText(buffer);
	VRSim3D::setUTF8Text(text,tempText);

	/// ʵ��������ػ�
	text->drawImplementation(renderInfo);
}

