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

	/// 设置文字信息的更新回调函数
	mInforText->setDrawCallback(new ValueTextDrawCallback(m_EarthManip.get()));

	/// 设置信息提示节点的属性，并关闭光照和深度测试
	osg::StateSet* stateset = viewPosInfoText->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	/// 将文本添加到绘制列表中
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
	/// 得到文字标注的实体指针
	osgText::Text* text = (osgText::Text*)(drawable);
	if(!text)
		return;

	/// 获取当前操纵器观察参数，并将其打印输出
	osgEarth::Util::Viewpoint vp = m_EarthManp->getViewpoint();

	char buffer[_MAX_PATH];
	sprintf(buffer,"经度:%6.2f  纬度:%6.2f  高度:%10.1f 俯仰:%5.2f 倾斜:%5.2f", 
		vp.x(), vp.y(), vp.getRange(), vp.getHeading(), vp.getPitch());
	std::string tempText(buffer);
	VRSim3D::setUTF8Text(text,tempText);

	/// 实现字体的重绘
	text->drawImplementation(renderInfo);
}

