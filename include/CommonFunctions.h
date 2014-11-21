//////////////////////////////////////////////////////////////////////////
///文件名：常用函数
///创建者：高志清
///创建时间：2013年7月19日
///内容描述：提供一些常用且比较独立函数
//////////////////////////////////////////////////////////////////////////

#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H 1

#include "Export.h"

#include <osg/AnimationPath>
#include <osg/Texture>
#include <osg/Camera>
#include <osgGA/GUIEventHandler>
#include <osgText/Text>
#include <osgUtil/LineSegmentIntersector>

namespace VRSim3D
{
	///zqgao 圆周运动
    extern osg::AnimationPathCallback* createAnimationPathCallback( float radius, float time );
	///zqgao RTTCamera
    extern osg::Camera* createRTTCamera( osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute=false );
	///zqgao HUDCamera
    extern osg::Camera* createHUDCamera( double left, double right, double bottom, double top );
	///zqgao 屏幕四边形
    extern osg::Geode* createScreenQuad( float width, float height, float scale=1.0f );
	///zqgao 文本
    extern osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size );
	///zqgao 文本以UTF8格式设置
	extern void setUTF8Text(osgText::Text* text,const std::string& content);
    ///zqgao 生成随即数
    extern float randomValue( float min, float max );
	///zqgao 随即坐标
    extern osg::Vec3 randomVector( float min, float max );
	///zqgao 随即MT
    extern osg::Matrix randomMatrix( float min, float max );
    
	///zqao 拣选事件
    class VRSim3D_EXPORT PickHandler : public osgGA::GUIEventHandler
    {
    public:
        virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
        virtual void doUserOperations( osgUtil::LineSegmentIntersector::Intersection& result ) = 0;
    };

}

#endif
