//////////////////////////////////////////////////////////////////////////
///�ļ��������ú���
///�����ߣ���־��
///����ʱ�䣺2013��7��19��
///�����������ṩһЩ�����ұȽ϶�������
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
	///zqgao Բ���˶�
    extern osg::AnimationPathCallback* createAnimationPathCallback( float radius, float time );
	///zqgao RTTCamera
    extern osg::Camera* createRTTCamera( osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute=false );
	///zqgao HUDCamera
    extern osg::Camera* createHUDCamera( double left, double right, double bottom, double top );
	///zqgao ��Ļ�ı���
    extern osg::Geode* createScreenQuad( float width, float height, float scale=1.0f );
	///zqgao �ı�
    extern osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size );
	///zqgao �ı���UTF8��ʽ����
	extern void setUTF8Text(osgText::Text* text,const std::string& content);
    ///zqgao �����漴��
    extern float randomValue( float min, float max );
	///zqgao �漴����
    extern osg::Vec3 randomVector( float min, float max );
	///zqgao �漴MT
    extern osg::Matrix randomMatrix( float min, float max );
    
	///zqao ��ѡ�¼�
    class VRSim3D_EXPORT PickHandler : public osgGA::GUIEventHandler
    {
    public:
        virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
        virtual void doUserOperations( osgUtil::LineSegmentIntersector::Intersection& result ) = 0;
    };

}

#endif
